/*
 * Copyright (c) 2018 Red Hat, Inc.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA. 
 *
 * $Id: //eng/uds-releases/flanders/src/uds/indexSession.h#2 $
 */

#ifndef INDEX_SESSION_H
#define INDEX_SESSION_H

#include "opaqueTypes.h"
#include "session.h"
#include "uds.h"
#include "util/atomic.h"

typedef enum {
  IS_INIT     = 1,
  IS_READY    = 2,
  IS_DISABLED = 3
} IndexSessionState;

/**
 * Structure corresponding to a UdsIndexSession
 **/
struct indexSession {
  Session   session;
  Atomic32  state;        // an atomically updated IndexSessionState value
  Grid     *grid;
};

/**
 * Check that the indexSession is usable.
 *
 * @param indexSession  the session to query
 *
 * @return UDS_SUCCESS or an error code
 **/
int checkIndexSession(IndexSession *indexSession)
  __attribute__((warn_unused_result));

/**
 * Get the current IndexSessionState from an index session.
 *
 * @param indexSession  the session to query
 **/
IndexSessionState getIndexSessionState(IndexSession *indexSession);

/**
 * Set the IndexSessionState of the IndexSession.
 *
 * @param indexSession  the session to be modified
 * @param state         the new session state
 **/
void setIndexSessionState(IndexSession      *indexSession,
                          IndexSessionState  state);

/**
 * Acquire pointer to the index session with the specified numeric ID.
 *
 * The pointer must eventually be released with a corresponding call to
 * releaseIndexSession().
 *
 * @param indexSessionID   The numeric ID of the desired session
 * @param indexSessionPtr  A pointer to receive the index session
 *
 * @return UDS_SUCCESS or an error code
 **/
int getIndexSession(unsigned int   indexSessionID,
                    IndexSession **indexSessionPtr)
  __attribute__((warn_unused_result));

/**
 * Release a pointer to an index session.
 *
 * @param indexSession  The session to release
 **/
void releaseIndexSession(IndexSession *indexSession);

/**
 * Construct a new index session, initializing the state to IS_INIT.
 *
 * @param indexSessionPtr   The pointer to receive the new session
 *
 * @return UDS_SUCCESS or an error code
 **/
int makeEmptyIndexSession(IndexSession **indexSessionPtr)
  __attribute__((warn_unused_result));

/**
 * Close the index session by saving the underlying grid, unloading the
 * modules referenced by the session, and freeing the underlying session
 * structure.
 *
 * @param indexSession  The index session to be shut down and freed
 **/
int saveAndFreeIndexSession(IndexSession *indexSession);

/**
 * Set the checkpoint frequency of the grid.
 *
 * @param session    The index session to be modified.
 * @param frequency  New checkpoint frequency.
 *
 * @return          Either UDS_SUCCESS or an error code.
 *
 **/
int udsSetCheckpointFrequency(UdsIndexSession session, unsigned int frequency)
  __attribute__((warn_unused_result));

#endif /* INDEX_SESSION_H */
