#include "obstacle.h"
#include "../game.h"
#include <iostream>
#include "stb_image.h"
#include "../util.h"

Obstacle::Obstacle(const std::string &texture, int xs, int ys, int w, int h, float threshold) :
    Node(), _threshold(threshold)
{
    int width, height, channels;
    auto assetFile = Game::instance().getWorkingDirectory() + "/assets/" + texture;

    unsigned char* data = stbi_load(assetFile.c_str(), &width, &height, &channels, 4); // Force 4 channels (RGBA)
    if (!data) {
        std::cerr << "Failed to load image: " << texture << std::endl;
    }
    std::vector<glm::vec2> outline;
    for (int x = 0; x < w; x++) {

        int y = 0;
        int index = ((ys + h) * width + (xs + x)) * 4;
        bool found = false;
        while (y < h) {
            // check alpha
            if (data[index + 3] > 9) {
                found= true;
                break;
            }

            y++;
            // moving up a row!
            index -= 4 * width;
        }
        if (found) {
            outline.push_back({(float)x, (float)y});
        }
    }

    for (auto v : outline) {
        std::cerr << v << "\n";
    }
    auto so = simplifyOutline(outline);
    std::cerr << " --- final outline ---\n";
    for (auto v : so) {
        std::cerr << v << "\n";
    }



}

std::vector<glm::vec2> Obstacle::simplifyOutline(const std::vector<glm::vec2> & points)
{
    std::vector<glm::vec2> simplified;
    simplified.push_back(points.front());

    int lastIndex = 0;
    for (int i = 1; i < points.size(); i++) {
        float maxError = 0.0f;
        int candidateIndex = lastIndex;

        for (int j = lastIndex + 1; j <= i; j++) {
            float expectedY = simplified.back().y + (points[j].x - simplified.back().x) *
                                                        (points[i].y - simplified.back().y) / (points[i].x - simplified.back().x);
            float error = fabs(points[j].y - expectedY);
            if (error > maxError) {
                maxError = error;
                candidateIndex = j;
            }
        }

        if (maxError > _threshold) {
            simplified.push_back(points[candidateIndex]);
            lastIndex = candidateIndex;
        }
    }

    simplified.push_back(points.back());
    return simplified;
}
