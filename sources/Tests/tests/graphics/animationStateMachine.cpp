#include "animation.h"


TEST_CASE("state-machine-variables-get-set", "[animation]") {
    using tests::MathUtils;

    auto skeleton = std::make_shared<Skeleton>(generateTestSkeleton());
    SkeletalAnimationStatesMachine stateMachine(skeleton);

    SkeletalAnimationStatesMachineVariables& variablesSet = stateMachine.getVariablesSet();

    SkeletalAnimationVariableId speedVarId = variablesSet.registerVariable("player_speed", 0.0f);
    variablesSet.setVariableValue("player_speed", 15.0f);

    REQUIRE(variablesSet.getVariableId("player_speed") == speedVarId);
    REQUIRE(MathUtils::isEqual(variablesSet.getVariableValue("player_speed"), 15.0f));
    REQUIRE(MathUtils::isEqual(variablesSet.getVariableValue(speedVarId), 15.0f));
}
