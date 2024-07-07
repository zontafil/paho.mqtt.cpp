/////////////////////////////////////////////////////////////////////////////
/// @file event.h
/// Declaration of MQTT event-related classes
/// @date July 6, 2024
/// @author Frank Pagliughi
/////////////////////////////////////////////////////////////////////////////

/*******************************************************************************
 * Copyright (c) 2024 Frank Pagliughi <fpagliughi@mindspring.com>
 *
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v2.0
 * and Eclipse Distribution License v1.0 which accompany this distribution.
 *
 * The Eclipse Public License is available at
 *    http://www.eclipse.org/legal/epl-v20.html
 * and the Eclipse Distribution License is available at
 *   http://www.eclipse.org/org/documents/edl-v10.php.
 *
 * Contributors:
 *    Frank Pagliughi - initial implementation and documentation
 *******************************************************************************/

#ifndef __mqtt_event_h
#define __mqtt_event_h

#include <variant>

#include "mqtt/message.h"
#include "mqtt/properties.h"
#include "mqtt/reason_code.h"
#include "mqtt/types.h"

namespace mqtt {

/////////////////////////////////////////////////////////////////////////////

/** Event for when the client is connected/reconnected */
struct connected_event
{
    string cause;
};

/** Event for when the connection is lost */
struct connection_lost_event
{
    string cause;
};

/** Event for when we receive a DISCONNECT packet from the server */
struct disconnected_event
{
    properties props;
    ReasonCode reasonCode;
};

/** Event for when a message arrives */
struct message_arrived_event
{
    const_message_ptr msg;
};

/** The variant type for any possible event. */
using event_type = std::variant<
    message_arrived_event, connected_event, connection_lost_event, disconnected_event>;

/////////////////////////////////////////////////////////////////////////////
}  // namespace mqtt

#endif  // __mqtt_event_h
