// test_response_options.cpp
//
// Unit tests for the response_options class in the Paho MQTT C++ library.
//

/*******************************************************************************
 * Copyright (c) 2016 Guilherme M. Ferreira <guilherme.maciel.ferreira@gmail.com>
 * Copyright (c) 2020-2023 Frank Pagliughi <fpagliughi@mindspring.com>
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
 *    Guilherme M. Ferreira
 *        - initial implementation and documentation
 *    Frank Pagliughi
 *        - converted to use Catch2
 *        - Merged in delivery response options
 *******************************************************************************/

#define UNIT_TESTS

#include <cstring>

#include "catch2_version.h"
#include "mock_async_client.h"
#include "mqtt/response_options.h"

using namespace mqtt;

/////////////////////////////////////////////////////////////////////////////

static constexpr token::Type TOKEN_TYPE = token::Type::CONNECT;

static mock_async_client cli;

// ----------------------------------------------------------------------
// Test default constructor
// ----------------------------------------------------------------------

TEST_CASE("response_options dflt constructor", "[options]")
{
    response_options opts;
    const auto& c_struct = opts.c_struct();

    REQUIRE(c_struct.context == nullptr);

    // Make sure the callback functions are set during object construction
    REQUIRE(c_struct.onSuccess != nullptr);
    REQUIRE(c_struct.onFailure != nullptr);
}

// ----------------------------------------------------------------------
// Test user constructor
// ----------------------------------------------------------------------

TEST_CASE("response_options user constructor", "[options]")
{
    token_ptr token{token::create(TOKEN_TYPE, cli)};
    response_options opts{token};
    const auto& c_struct = opts.c_struct();

    REQUIRE(c_struct.context == token.get());

    // Make sure the callback functions are set during object construction
    REQUIRE(c_struct.onSuccess != nullptr);
    REQUIRE(c_struct.onFailure != nullptr);
}

// ----------------------------------------------------------------------
// Test copy constructor
// ----------------------------------------------------------------------

TEST_CASE("response_options copy constructor", "[options]")
{
    token_ptr token{token::create(TOKEN_TYPE, cli)};
    response_options opts_org{token};

    properties props{
        {property::PAYLOAD_FORMAT_INDICATOR, 42}, {property::MESSAGE_EXPIRY_INTERVAL, 70000}
    };
    opts_org.set_properties(props);

    response_options opts{opts_org};

    const auto& copts = opts.c_struct();

    REQUIRE(copts.context == token.get());

    // Make sure the callback functions are set during object construction
    REQUIRE(copts.onSuccess != nullptr);
    REQUIRE(copts.onFailure != nullptr);

    REQUIRE(opts.get_properties().size() == 2);
}

// ----------------------------------------------------------------------
// Test builder
// ----------------------------------------------------------------------

TEST_CASE("response_options builder", "[options]")
{
    token_ptr token{token::create(TOKEN_TYPE, cli)};

    properties props{
        {property::PAYLOAD_FORMAT_INDICATOR, 42}, {property::MESSAGE_EXPIRY_INTERVAL, 70000}
    };

    auto opts =
        response_options_builder().mqtt_version(5).token(token).properties(props).finalize();
}

// ----------------------------------------------------------------------
// Test set context
// ----------------------------------------------------------------------

TEST_CASE("response_options set token", "[options]")
{
    response_options opts;
    const auto& c_struct = opts.c_struct();

    REQUIRE(c_struct.context == nullptr);
    token_ptr token{token::create(TOKEN_TYPE, cli)};
    opts.set_token(token);
    REQUIRE(c_struct.context == token.get());
}

/////////////////////////////////////////////////////////////////////////////
// Delivery Response Options
/////////////////////////////////////////////////////////////////////////////

// ----------------------------------------------------------------------
// Test default constructor
// ----------------------------------------------------------------------

TEST_CASE("delivery_response_options dflt constructor", "[options]")
{
    delivery_response_options opts;
    const auto& c_struct = opts.c_struct();

    REQUIRE(c_struct.context == nullptr);

    // Make sure the callback functions are set during object construction
    REQUIRE(c_struct.onSuccess != nullptr);
    REQUIRE(c_struct.onFailure != nullptr);
}

// ----------------------------------------------------------------------
// Test user constructor
// ----------------------------------------------------------------------

TEST_CASE("delivery_response_options user constructor", "[options]")
{
    mock_async_client cli;

    delivery_token_ptr token{new delivery_token{cli}};
    delivery_response_options opts{token};
    const auto& c_struct = opts.c_struct();

    REQUIRE(c_struct.context == token.get());

    // Make sure the callback functions are set during object construction
    REQUIRE(c_struct.onSuccess != nullptr);
    REQUIRE(c_struct.onFailure != nullptr);
}

// ----------------------------------------------------------------------
// Test set context
// ----------------------------------------------------------------------

TEST_CASE("delivery_response_options set token", "[options]")
{
    delivery_response_options opts;
    const auto& c_struct = opts.c_struct();

    REQUIRE(c_struct.context == nullptr);

    mock_async_client cli;
    delivery_token_ptr token{new delivery_token{cli}};
    opts.set_token(token);
    REQUIRE(c_struct.context == token.get());
}
