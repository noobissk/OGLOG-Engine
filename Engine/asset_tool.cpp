#include <service/asset_manager.h>
#include <iostream>

int main()
{
    std::cout << "=== Asset Tool ===" << std::endl;
    std::cout << "Scanning and updating assets.sdb..." << std::endl;
    
    AssetManager::start(true);
    
    std::cout << "Done! Assets database updated." << std::endl;
    
    std::string dummy;
    std::cout << "Press enter to exit..." << std::endl;
    std::getline(std::cin, dummy);
    
    return 0;
}
