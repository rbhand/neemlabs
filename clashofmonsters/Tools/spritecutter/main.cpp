//
//  main.cpp
//  spritesheetgen
//
//  Created by Vitaliy Ivanov on 6/26/12.
//

#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <vector>
#include <memory>

#include "types.h"
#include "frame.h"
#include "platform.h"

#ifdef DEBUG
#define SSG_DEBUG_LOG
#endif

using namespace ssg;
using namespace std;

int main(int argc, const char * argv[])
{
	on_start();
	
	if (argc < 2 || (argc >= 2 && string(argv[1]) == "--help"))
	{
		cout << argv[0] << "\"<input-dir-path 1>\" \"[input-dir-path 2]\" ... \"[input-dir-path n]\"\n";
		
		on_end();
		return -1;
	}
	
	FramesList frames;
	for (int i=1; i<argc; ++i)
	{
		string input_dir = argv[i];
		cout << "Reading frames from: \"" << input_dir << "\" ..." << endl;
		
		FramesList new_frames = frames_from_dir(input_dir);
		cout << "  " << new_frames.size() << " found" << endl;
		frames.insert(frames.end(), new_frames.begin(), new_frames.end());
	}
	
	if (frames.empty())
	{
		cout << "ERROR: No frames found" << endl;
	
		on_end();
		return -1;
	}
	
	rect_t rect = frames.front()->imageBounds;
	for (FramesList::const_iterator i=frames.begin(), e=frames.end(); i!=e; ++i)
	{
		const Frame& f = **i;
		rect = rect.united(f.imageBounds);
	}
	
	int div = rect.width % 4;
	if (div != 0)
	{
		rect.width += (4 - div);
		rect.x -= (4 - div) / 2;
	}
	div = rect.height % 4;
	if (div != 0)
	{
		rect.height += (4 - div);
		rect.y -= (4 - div) / 2;
	}
	
	
	cout << "Cropping frames..." << endl;
	for (FramesList::const_iterator i=frames.begin(), e=frames.end(); i!=e; ++i)
	{
		cout << "  " << (**i).absolutePath << endl;
		write_cropped_frame(**i, rect, (**i).absolutePath);
		//write_cropped_frame(**i, rect, (**i).absoluteDirPath + "/cropped/" + (**i).imageName);
	}
	
	on_end();
	return 0;
}
