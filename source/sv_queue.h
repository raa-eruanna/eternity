// Emacs style mode select -*- C++ -*- vi:sw=3 ts=3:
//----------------------------------------------------------------------------
//
// Copyright(C) 2011 Charles Gunyon
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
//   Serverside player queue routines.
//
//----------------------------------------------------------------------------

#ifndef SV_QUEUE_H__
#define SV_QUEUE_H__

void SV_QueueSetClientRemoved(int clientnum);
void SV_QueueSetClientWaiting(int clientnum, int position);
void SV_QueueSetClientDoneWaiting(int clientnum);
void SV_QueueSetClientCanJoin(int clientnum);
void SV_QueueSetClientNotPlaying(int clientnum);
void SV_QueueSetClientPlaying(int clientnum);
void SV_PutClientInQueue(int clientnum);
void SV_MarkQueueClientsAFK();

#endif
