#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <optional>
#include <cmath>

#include "../common/components/tile.h"
#include "../common/io/filing.h"

constexpr unsigned int TILE_SIZE = 16;
constexpr unsigned int VIEWPORT_WIDTH = 640;
constexpr unsigned int VIEWPORT_HEIGHT = 480;
constexpr unsigned int PALETTE_WIDTH = 256;
constexpr unsigned int WINDOW_WIDTH = VIEWPORT_WIDTH + PALETTE_WIDTH;
constexpr unsigned int WINDOW_HEIGHT = VIEWPORT_HEIGHT;

int main()
{
    sf::RenderWindow window(sf::VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}), "SFML 3.0 Map Editor - Layer & Canvas Controls");
    window.setFramerateLimit(60);

    sf::Texture tilemap;
    if (!tilemap.loadFromFile("../resource/tilemap.png"))
    {
        std::cerr << "Failed to load ../resources/tilemap.png\n";
        return -1;
    }

    const unsigned int tilesPerRow = tilemap.getSize().x / TILE_SIZE;
    const unsigned int tilesPerCol = tilemap.getSize().y / TILE_SIZE;
    const unsigned int totalTiles = tilesPerRow * tilesPerCol;

    // Map Dimensions (Adjustable at runtime)
    unsigned int mapGridWidth = 40;
    unsigned int mapGridHeight = 30;

    // Editor State
    std::vector<Tile> mapData;
    uint8_t selectedId = 0;
    uint8_t currentLayer = 0;
    uint8_t maxLayerCreated = 0;
    uint8_t currentPhysics = 0;
    bool showAllLayers = true;

    // Canvas Camera offset for panning large maps
    sf::Vector2f cameraOffset{0.0f, 0.0f};
    const float panSpeed = 16.0f;

    auto getTileRect = [tilesPerRow](uint8_t id) -> sf::IntRect {
        int tx = (id % tilesPerRow) * TILE_SIZE;
        int ty = (id / tilesPerRow) * TILE_SIZE;
        return sf::IntRect({tx, ty}, {static_cast<int>(TILE_SIZE), static_cast<int>(TILE_SIZE)});
    };

    while (window.isOpen())
    {
        while (const std::optional<sf::Event> event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }
            else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
            {
                // File Operations
                if (keyPressed->code == sf::Keyboard::Key::F5)
                {
                    if (filing::saveToFile("../resource/map.bin", mapData))
                        std::cout << "[Save] Saved " << mapData.size() << " tiles to map.bin\n";
                }
                else if (keyPressed->code == sf::Keyboard::Key::F9)
                {
                    mapData.clear();
                    if (filing::loadFromFile("../resource/map.bin", mapData))
                    {
                        std::stable_sort(mapData.begin(), mapData.end());
                        for (const auto& t : mapData)
                            maxLayerCreated = std::max(maxLayerCreated, t.layer);
                        std::cout << "[Load] Loaded " << mapData.size() << " tiles. Highest layer: " 
                                  << static_cast<int>(maxLayerCreated) << "\n";
                    }
                }

                // Layer Management
                else if (keyPressed->code == sf::Keyboard::Key::Equal) // '+' key
                {
                    if (currentLayer < 255)
                    {
                        currentLayer++;
                        maxLayerCreated = std::max(maxLayerCreated, currentLayer);
                        std::cout << "Active Layer: " << static_cast<int>(currentLayer) 
                                  << " (Max Created: " << static_cast<int>(maxLayerCreated) << ")\n";
                    }
                }
                else if (keyPressed->code == sf::Keyboard::Key::Hyphen) // '-' key
                {
                    if (currentLayer > 0)
                    {
                        currentLayer--;
                        std::cout << "Active Layer: " << static_cast<int>(currentLayer) << "\n";
                    }
                }
                else if (keyPressed->code == sf::Keyboard::Key::V)
                {
                    showAllLayers = !showAllLayers;
                    std::cout << (showAllLayers ? "Displaying: All Layers\n" : "Displaying: Active Layer Only\n");
                }

                // Dynamic Canvas Resizing
                else if (keyPressed->code == sf::Keyboard::Key::Right)
                {
                    mapGridWidth++;
                    std::cout << "Map Size: " << mapGridWidth << " x " << mapGridHeight << "\n";
                }
                else if (keyPressed->code == sf::Keyboard::Key::Left)
                {
                    if (mapGridWidth > 1)
                    {
                        mapGridWidth--;
                        // Prune any tiles outside the new border
                        std::erase_if(mapData, [mapGridWidth](const Tile& t) { return t.x >= mapGridWidth; });
                        std::cout << "Map Size: " << mapGridWidth << " x " << mapGridHeight << "\n";
                    }
                }
                else if (keyPressed->code == sf::Keyboard::Key::Down)
                {
                    mapGridHeight++;
                    std::cout << "Map Size: " << mapGridWidth << " x " << mapGridHeight << "\n";
                }
                else if (keyPressed->code == sf::Keyboard::Key::Up)
                {
                    if (mapGridHeight > 1)
                    {
                        mapGridHeight--;
                        // Prune any tiles outside the new border
                        std::erase_if(mapData, [mapGridHeight](const Tile& t) { return t.y >= mapGridHeight; });
                        std::cout << "Map Size: " << mapGridWidth << " x " << mapGridHeight << "\n";
                    }
                }

                // Physics Flag Toggle
                else if (keyPressed->code == sf::Keyboard::Key::P)
                {
                    currentPhysics = (currentPhysics == 0) ? 1 : 0;
                    std::cout << "Tile Physics set to: " << static_cast<int>(currentPhysics) << "\n";
                }
            }
        }

        // Camera Pan Controls
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) cameraOffset.y += panSpeed;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) cameraOffset.y -= panSpeed;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) cameraOffset.x += panSpeed;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) cameraOffset.x -= panSpeed;

        sf::Vector2i mousePos = sf::Mouse::getPosition(window);

        // Palette Tile Selection
        if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
        {
            if (mousePos.x >= static_cast<int>(VIEWPORT_WIDTH) && mousePos.x < static_cast<int>(WINDOW_WIDTH))
            {
                int px = (mousePos.x - VIEWPORT_WIDTH) / TILE_SIZE;
                int py = mousePos.y / TILE_SIZE;

                if (px < static_cast<int>(tilesPerRow) && py < static_cast<int>(tilesPerCol))
                {
                    unsigned int id = py * tilesPerRow + px;
                    if (id < totalTiles && id <= 255)
                    {
                        selectedId = static_cast<uint8_t>(id);
                    }
                }
            }
        }

        // Viewport Placement / Removal with Camera & Bounds Handling
        if (mousePos.x >= 0 && mousePos.x < static_cast<int>(VIEWPORT_WIDTH) &&
            mousePos.y >= 0 && mousePos.y < static_cast<int>(WINDOW_HEIGHT))
        {
            float worldX = static_cast<float>(mousePos.x) - cameraOffset.x;
            float worldY = static_cast<float>(mousePos.y) - cameraOffset.y;

            if (worldX >= 0.0f && worldY >= 0.0f)
            {
                auto gridX = static_cast<uint16_t>(worldX / TILE_SIZE);
                auto gridY = static_cast<uint16_t>(worldY / TILE_SIZE);

                if (gridX < mapGridWidth && gridY < mapGridHeight)
                {
                    if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
                    {
                        // Replace existing tile on this specific layer and coordinate
                        std::erase_if(mapData, [gridX, gridY, currentLayer](const Tile& t) {
                            return t.x == gridX && t.y == gridY && t.layer == currentLayer;
                        });

                        mapData.push_back(Tile{
                            .layer = currentLayer,
                            .size = static_cast<uint8_t>(TILE_SIZE),
                            .physics = currentPhysics,
                            .id = selectedId,
                            .x = gridX,
                            .y = gridY
                        });

                        std::stable_sort(mapData.begin(), mapData.end());
                    }
                    else if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Right))
                    {
                        std::erase_if(mapData, [gridX, gridY, currentLayer](const Tile& t) {
                            return t.x == gridX && t.y == gridY && t.layer == currentLayer;
                        });
                    }
                }
            }
        }

        // Clear Background
        window.clear(sf::Color(25, 25, 25));

        // 1. Draw Canvas Boundary Box
        sf::RectangleShape canvasBorder({
            static_cast<float>(mapGridWidth * TILE_SIZE),
            static_cast<float>(mapGridHeight * TILE_SIZE)
        });
        canvasBorder.setPosition(cameraOffset);
        canvasBorder.setFillColor(sf::Color(40, 40, 40));
        canvasBorder.setOutlineColor(sf::Color(80, 80, 80));
        canvasBorder.setOutlineThickness(1.0f);
        window.draw(canvasBorder);

        // 2. Draw Map Tiles
        sf::Sprite sprite(tilemap);
        for (const auto& tile : mapData)
        {
            // Skip other layers if visibility filter is active
            if (!showAllLayers && tile.layer != currentLayer)
                continue;

            sprite.setTextureRect(getTileRect(tile.id));
            sprite.setPosition({
                cameraOffset.x + static_cast<float>(tile.x * TILE_SIZE),
                cameraOffset.y + static_cast<float>(tile.y * TILE_SIZE)
            });

            // Dim inactive layers if all layers are displayed
            if (showAllLayers && tile.layer != currentLayer)
                sprite.setColor(sf::Color(255, 255, 255, 120));
            else
                sprite.setColor(sf::Color(255, 255, 255, 255));

            window.draw(sprite);
        }

        // 3. Tile Hover Placement Preview
        if (mousePos.x >= 0 && mousePos.x < static_cast<int>(VIEWPORT_WIDTH) &&
            mousePos.y >= 0 && mousePos.y < static_cast<int>(WINDOW_HEIGHT))
        {
            float worldX = static_cast<float>(mousePos.x) - cameraOffset.x;
            float worldY = static_cast<float>(mousePos.y) - cameraOffset.y;

            if (worldX >= 0.0f && worldY >= 0.0f)
            {
                auto gridX = static_cast<uint16_t>(worldX / TILE_SIZE);
                auto gridY = static_cast<uint16_t>(worldY / TILE_SIZE);

                if (gridX < mapGridWidth && gridY < mapGridHeight)
                {
                    sf::Sprite preview(tilemap);
                    preview.setTextureRect(getTileRect(selectedId));
                    preview.setPosition({
                        cameraOffset.x + static_cast<float>(gridX * TILE_SIZE),
                        cameraOffset.y + static_cast<float>(gridY * TILE_SIZE)
                    });
                    preview.setColor(sf::Color(255, 255, 255, 180));
                    window.draw(preview);
                }
            }
        }

        // 4. Sidebar Palette & Dividing Line
        sf::RectangleShape sidebarBg({static_cast<float>(PALETTE_WIDTH), static_cast<float>(WINDOW_HEIGHT)});
        sidebarBg.setPosition({static_cast<float>(VIEWPORT_WIDTH), 0.0f});
        sidebarBg.setFillColor(sf::Color(35, 35, 35));
        window.draw(sidebarBg);

        sf::RectangleShape separator({2.0f, static_cast<float>(WINDOW_HEIGHT)});
        separator.setPosition({static_cast<float>(VIEWPORT_WIDTH), 0.0f});
        separator.setFillColor(sf::Color(100, 100, 100));
        window.draw(separator);

        sf::Sprite paletteSprite(tilemap);
        paletteSprite.setPosition({static_cast<float>(VIEWPORT_WIDTH + 8), 8.0f});
        window.draw(paletteSprite);

        // Highlight selected palette tile
        sf::RectangleShape selectionBox({static_cast<float>(TILE_SIZE), static_cast<float>(TILE_SIZE)});
        selectionBox.setFillColor(sf::Color::Transparent);
        selectionBox.setOutlineColor(sf::Color::Red);
        selectionBox.setOutlineThickness(1.0f);
        selectionBox.setPosition({
            static_cast<float>(VIEWPORT_WIDTH + 8 + (selectedId % tilesPerRow) * TILE_SIZE),
            static_cast<float>(8 + (selectedId / tilesPerRow) * TILE_SIZE)
        });
        window.draw(selectionBox);

        window.display();
    }

    return 0;
}