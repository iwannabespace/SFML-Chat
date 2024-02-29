#include "../include/functions.hpp"
#include "../../Shared/shared.hpp"
#include <fstream>
#include <filesystem>
#include <sstream>

namespace Functions
{
    void FitText(sf::Text& text, sf::Vector2f size)
    {
        for (uint8_t i = 1; true; i++)
        {
            text.setCharacterSize(i);
            
            auto tSize = text.getGlobalBounds().getSize();
            
            if (tSize.x >= size.x || tSize.y >= size.y)
            {
                text.setCharacterSize(i - 24);
                break;
            }
        }
    }

    float GetMiddle(float object, float container, float start, float minus)
    {
        return std::round(start + ((container - object) / 2) - minus);
    }
    
    std::string ConvertTimeToText(int32_t duration)
    {
        std::string text;
        std::string minutes = std::to_string(duration / 60);
        std::string seconds = std::to_string(duration % 60);
        
        if (minutes.length() == 1)
            text += "0";
        text += (minutes + ":");

        if (seconds.length() == 1)
            text += "0";
        text += seconds;

        return text;
    }

    sf::Color GetColorFromTextbox(const std::string& r, const std::string& g, const std::string& b)
    {
            std::uint8_t rc = 0, gc = 0, bc = 0;

            if (r.length())
                rc = std::stoul(r);
            
            if (g.length())
                gc = std::stoul(g);
            
            if (b.length())
                bc = std::stoul(b);

            return { rc, gc, bc };
    }

    float GetTextMaxHeight(const sf::Text& text)
    {
        sf::Text temp;
        temp.setFont(*text.getFont());
        temp.setString("qwertyuıopğüasdfghjklşizxcvbnmöçQWERTYUIOPĞÜASDFGHJKLŞİZXCVBNMÖÇ");
        temp.setCharacterSize(text.getCharacterSize());
        
        return temp.getGlobalBounds().height;
    }

    std::string GetFileName()
    {
        uint32_t index = 0;

        if (!std::filesystem::exists("files/index_holder.txt"))
        {
            std::ofstream file("files/index_holder.txt", std::ios::out);
            file << 1;
            file.close();
        }

        else
        {
            std::ifstream inFile("files/index_holder.txt");
            inFile >> index;
            inFile.close();
            std::ofstream outFile("files/index_holder.txt", std::ios::out);
            outFile << index + 1;
            outFile.close();
        }

        return "file" + std::to_string(index);
    }

    void AddNewLinesToText(sf::Text& text, uint32_t maxWidth)
    {
        std::string str = text.getString();
        std::string temp;

        for (size_t i = 0; i < str.length(); i++) {
            char c = str[i];
            
            if (sf::Text(temp + c, *text.getFont(), text.getCharacterSize()).getGlobalBounds().width < maxWidth) {
                temp += c;
            } else {
                temp += '\n';
                temp += c;
            }
        }

        text.setString(temp);
    }

    Message CreateMessage(uint64_t id, uint8_t type, const std::string& data, const std::string& fileExtension)
    {
        if (type == Shared::TEXT) {
            return { id, {}, data, MessageType::Text };
        } else {
            std::string filename = GetFileName() + fileExtension;
            std::string path = "files/" + filename;

            std::fstream file(path, std::ios::out | std::ios::binary);
            file << data;
            file.close();

            return { id, {}, path, type == Shared::FILE ? MessageType::File : MessageType::Sound };
        }
    }

    std::string ReadFileAsBinary(const std::string& filename)
    {
        std::ifstream file(filename, std::ios::binary);

        std::ostringstream oss;
        oss << file.rdbuf();

        return oss.str();
    }
}