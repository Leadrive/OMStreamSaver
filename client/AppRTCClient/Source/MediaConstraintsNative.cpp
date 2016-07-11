/* (c) Copyright 2014 Temasys Communication, Pte Ltd.

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and 
   limitations under the License.

 *
 * MediaConstraintsNative.cpp
 *
 *  Created on: Nov 14, 2013
 *      Author: Francois Temasys
 */
#include "MediaConstraintsNative.h"

MediaConstraintsNative::
~MediaConstraintsNative()
 {}

MediaConstraintsNative::
MediaConstraintsNative()
 {}

MediaConstraintsNative::
MediaConstraintsNative(
  const MediaConstraintsInterface::Constraints& mandatory,
  const MediaConstraintsInterface::Constraints& optional
) :
  mandatory_( mandatory ),
  optional_(  optional  )
{}

const webrtc::MediaConstraintsInterface::Constraints&
MediaConstraintsNative::
GetMandatory() const
{
  return mandatory_;
}

const webrtc::MediaConstraintsInterface::Constraints&
MediaConstraintsNative::
GetOptional() const
{
  return optional_;
}
