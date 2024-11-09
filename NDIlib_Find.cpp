#include <iostream>
#include <cstdio>
#include <chrono>
#include <unordered_map>
#include <locale>
#include <fstream>
#include <Processing.NDI.Lib.h>

#ifdef _WIN32
#ifdef _WIN64
#pragma comment(lib, "Processing.NDI.Lib.x64.lib")
#else // _WIN64
#pragma comment(lib, "Processing.NDI.Lib.x86.lib")
#endif // _WIN64
#endif // _WIN32

bool islastCharNumber(const std::string& inStr)
{
  std::locale loc;
  if (std::isdigit(inStr.end(), loc))
    return true;
  else
    return false;
}

void saveSourceToFile(const std::string fileName, const std::unordered_map<int, std::string>& sourceMap)
{
  std::ofstream file(fileName);
  if(file.is_open())
  {
    //header of the file
    file<<"----------------------------------------------------------\n";
    file<<"Following are the sources with name ending with number :\n";
    file<<"----------------------------------------------------------\n";
    
    //names of the sources
    for(auto current : sourceMap)
      file<<current.first<<" : "<<current.second<<"\n";
      
    //end of the file
    file<<"----------------------------------------------------------\n";
  }
}


int main(int argc, char* argv[])
{
	// Not required, but "correct" (see the SDK documentation).
	if (!NDIlib_initialize())
		return 0;

	// We are going to create an NDI finder that locates sources on the network.
	NDIlib_find_instance_t pNDI_find = NDIlib_find_create_v2();
	if (!pNDI_find)
		return 0;
        //unordered map to store sources name which has name ending with number
        std::unordered_map<int, std::string> numberedSources;
        int countNumberedSources=0;

	// Run for one minute
	using namespace std::chrono;
	for (const auto start = high_resolution_clock::now(); high_resolution_clock::now() - start < minutes(1);) {
		// Wait up till 5 seconds to check for new sources to be added or removed
		if (!NDIlib_find_wait_for_sources(pNDI_find, 5000 /* milliseconds */)) {
			std::cout<<"No change to the sources found.\n";
			continue;
		}

		// Get the updated list of sources
		uint32_t no_sources = 0;
		const NDIlib_source_t* p_sources = NDIlib_find_get_current_sources(pNDI_find, &no_sources);

		// Display all the sources.
		std::cout<<"Network sources ("<<no_sources<<" found).\n";
		for (uint32_t i = 0; i < no_sources; i++)
		{
		        std::string sourceName = p_sources[i].p_ndi_name;
			std::cout<<i + 1<<". "<< sourceName<<std::endl;
			if(islastCharNumber(sourceName))
			  numberedSources[++countNumberedSources] = sourceName;
			
		}
	}
	
	//save source name to the file
	saveSourceToFile("foundSourcesList.txt", numberedSources);

	// Destroy the NDI finder
	NDIlib_find_destroy(pNDI_find);

	// Finished
	NDIlib_destroy();

	// Success. We are done
	return 0;
}
