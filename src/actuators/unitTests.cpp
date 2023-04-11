//#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "../catch.hpp"
#include "LinearActuator.hpp"  // include the header file for LinearActuator class

TEST_CASE( "LinearActuator moves correctly in cm", "[LinearActuator]" ) {
    LinearActuator actuator;
    actuator.moveCM(20.0);
    REQUIRE( actuator.getPositionCM() == Approx(20.0) );

    actuator.moveCM(40.0);
    REQUIRE( actuator.getPositionCM() == Approx(40.0) );

    REQUIRE_THROWS_AS( actuator.moveCM(0.5), std::invalid_argument );
    REQUIRE_THROWS_AS( actuator.moveCM(50.0), std::invalid_argument );
}

TEST_CASE( "LinearActuator moves correctly in inch", "[LinearActuator]" ) {
    LinearActuator actuator;
    actuator.moveInch(10.0);
    REQUIRE( actuator.getPositionInch() == Approx(10.0) );

    actuator.moveInch(40.0);
    REQUIRE( actuator.getPositionInch() == Approx(40.0) );

    REQUIRE_THROWS_AS( actuator.moveInch(0.5), std::invalid_argument );
    REQUIRE_THROWS_AS( actuator.moveInch(50.0), std::invalid_argument );
}
