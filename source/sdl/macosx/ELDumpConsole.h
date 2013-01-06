// Emacs style mode select -*- Objective-C -*-
//----------------------------------------------------------------------------
//
// Copyright(C) 2012 Ioan Chera
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
//----------------------------------------------------------------------------
//
// DESCRIPTION:
//
// Dump window for console output 
//
//----------------------------------------------------------------------------

#import <Cocoa/Cocoa.h>


@interface ELDumpConsole : NSWindowController
{
	IBOutlet NSTextView *textField;			// the text display
	IBOutlet NSPanel *pwindow;					// the console panel
	IBOutlet NSView *errorMessage;			// the error message icon + label
   NSPipe *pipe;									// i/o pipe with eternity engine
	NSFileHandle *inHandle;						// the pipe file handle
	id masterOwner;								// LauncherController
	NSMutableString *outputMessageString;	// what gets received
	IBOutlet NSTextField *errorLabel;		// text label with error message
}

@property (assign) id masterOwner;

-(id)initWithWindowNibName:(NSString *)windowNibName;
-(void)startLogging:(NSTask *)engineTask;
-(void)dataReady:(NSNotification *)notification;

@end

// EOF

