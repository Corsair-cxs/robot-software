#include <string.h>

#include "state.h"

RobotState initial_state()
{
    RobotState state;

    return state;
}

bool operator==(const RobotState& lhs, const RobotState& rhs)
{
    return !memcmp(&lhs, &rhs, sizeof(RobotState));
}
