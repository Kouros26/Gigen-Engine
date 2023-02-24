#include <Quaternion/FQuat.hpp>

#define GLM_FORCE_SILENT_WARNINGS
#define GLM_FORCE_XYZW_ONLY
#include <glm/gtc/quaternion.hpp>

#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>

using namespace lm;

#define CHECK_QUAT(quat, quatGlm) CHECK(quat.x == Catch::Approx(quatGlm.x)); CHECK(quat.y == Catch::Approx(quatGlm.y)); CHECK(quat.z == Catch::Approx(quatGlm.z)); CHECK(quat.w == Catch::Approx(quatGlm.w))

TEST_CASE("Quaternion", "[.all][Quaternion]")
{
    SECTION("Instantiation")
    {
        // Default Constructor
        lm::FQuat empty;
        glm::quat emptyGlm{};

        CHECK_QUAT(empty, emptyGlm);

        // Destructor
        {
            lm::FQuat temp;
        }

        // Basic Constructor
        lm::FQuat allParam{ .5f, 2.f, 5.f, 2.5f }; // Axis X, Axis, Y, Axis Z, Angle W
        glm::quat allParamGlm{ 2.5f, .5f, 2.f, 5.f }; // Angle W, Axis X, Axis, Y, Axis Z
        CHECK_QUAT(allParam, allParamGlm);

        // Copy Constructor
        lm::FQuat copy{ allParam };
        glm::quat copyGlm{ allParamGlm };
        CHECK_QUAT(copy, copyGlm);

        // Assignment Operator
        empty = allParam;
        emptyGlm = allParamGlm;
        CHECK_QUAT(empty, emptyGlm);

        // Size compare
        CHECK(sizeof lm::FQuat == sizeof glm::quat);

        // Memory
        CHECK(memcmp(&allParam, &allParamGlm, sizeof lm::FQuat) == 0);
    }

    SECTION("Accessor")
    {
        SECTION("Index Operator")
        {
            lm::FQuat quat{ 1.f, 2.f, 3.f, 4.f };
            glm::quat quatGlm{ 4.f, 1.f, 2.f, 3.f };

            CHECK_QUAT(quat, quatGlm);

            quat[0] = 5.f;
            quatGlm[0] = 5.f;
            CHECK_QUAT(quat, quatGlm);

            quat[1] = 6.f;
            quatGlm[1] = 6.f;
            CHECK_QUAT(quat, quatGlm);

            quat[2] = 7.f;
            quatGlm[2] = 7.f;
            CHECK_QUAT(quat, quatGlm);

            quat[3] = 8.f;
            quatGlm[3] = 8.f;
            CHECK_QUAT(quat, quatGlm);

            CHECK(quat[0] == Catch::Approx(quatGlm[0]));
            CHECK(quat[1] == Catch::Approx(quatGlm[1]));
            CHECK(quat[2] == Catch::Approx(quatGlm[2]));
            CHECK(quat[3] == Catch::Approx(quatGlm[3]));
        }

        SECTION("X")
        {
            lm::FQuat quat{ 1.f, 2.f, 3.f, 4.f };
            glm::quat quatGlm{ 4.f, 1.f, 2.f, 3.f };

            CHECK_QUAT(quat, quatGlm);

            quat.x = 5.f;
            quatGlm.x = 5.f;
            CHECK_QUAT(quat, quatGlm);

            CHECK(quat.x == Catch::Approx(quatGlm.x));
        }

        SECTION("Y")
        {
            lm::FQuat quat{ 1.f, 2.f, 3.f, 4.f };
            glm::quat quatGlm{ 4.f, 1.f, 2.f, 3.f };

            CHECK_QUAT(quat, quatGlm);

            quat.y = 5.f;
            quatGlm.y = 5.f;
            CHECK(quat.y == Catch::Approx(quatGlm.y));
        }

        SECTION("Z")
        {
            lm::FQuat quat{ 1.f, 2.f, 3.f, 4.f };
            glm::quat quatGlm{ 4.f, 1.f, 2.f, 3.f };

            CHECK_QUAT(quat, quatGlm);

            quat.z = 5.f;
            quatGlm.z = 5.f;
            CHECK_QUAT(quat, quatGlm);

            CHECK(quat.z == Catch::Approx(quatGlm.z));
        }
    }

    SECTION("Comparator")
    {
        // Compare with self
        lm::FQuat self{ 1.f, 2.f, 3.f, 4.f };
        CHECK(self == self);

        // Compare with self
        CHECK(lm::FQuat{ 1.f, 2.f, 3.f, 4.f } == lm::FQuat{ 1.f, 2.f, 3.f, 4.f });

        // trying to find copy/paste mistake
        lm::FQuat twos{ 2.f, 2.f,2.f,2.f };
        CHECK_FALSE(lm::FQuat{ .5f, 2.f, 2.f, 2.f } == twos);
        CHECK_FALSE(lm::FQuat{ 2.f, .5f, 2.f, 2.f } == twos);
        CHECK_FALSE(lm::FQuat{ 2.f, 2.f, .5f, 2.f } == twos);
        CHECK_FALSE(lm::FQuat{ 2.f, 2.f, 2.f, .5f } == twos);
        CHECK_FALSE(twos == lm::FQuat{ .5f, 2.f, 2.f, 2.f });
        CHECK_FALSE(twos == lm::FQuat{ 2.f, .5f, 2.f, 2.f });
        CHECK_FALSE(twos == lm::FQuat{ 2.f, 2.f, .5f, 2.f });
        CHECK_FALSE(twos == lm::FQuat{ 2.f, 2.f, 2.f, .5f });

        // Testing comparison
        CHECK_FALSE(self.isUnit());
        CHECK(lm::FQuat{ sqrtf(0.25f), sqrtf(0.25f), sqrtf(0.25f), sqrtf(0.25f) }.isUnit());
    }

    SECTION("Arithmetic")
    {
        lm::FQuat const small{ 2.f, 3.f, 4.f, 1.f };
        lm::FQuat const big{ 3.f, 2.f, 1.f, 4.f, };

        glm::quat const smallGlm{ 1.f, 2.f, 3.f, 4.f };
        glm::quat const bigGlm{ 4.f, 3.f, 2.f, 1.f };

        SECTION("Addition")
        {
            {
                lm::FQuat sum = big;
                sum += small;

                glm::quat sumGlm = bigGlm;
                sumGlm += smallGlm;

                CHECK_QUAT(sum, sumGlm);
            }

            {
                lm::FQuat sum = big + small;

                glm::quat sumGlm = bigGlm + smallGlm;

                CHECK_QUAT(sum, sumGlm);
            }
        }

        SECTION("Opposite")
        {
            lm::FQuat opposite = -big;

            glm::quat oppositeGlm = -bigGlm;

            CHECK_QUAT(opposite, oppositeGlm);
        }

        SECTION("Subtraction")
        {
            lm::FQuat difference = big - small;

            glm::quat differenceGlm = bigGlm - smallGlm;

            CHECK_QUAT(difference, differenceGlm);
        }

        SECTION("Multiplication")
        {
            {
                lm::FQuat product = big * small;

                glm::quat productGlm = bigGlm * smallGlm;

                CHECK_QUAT(product, productGlm);
            }

            {
                lm::FQuat product = big * 2.f;

                glm::quat productGlm = bigGlm * 2.f;

                CHECK_QUAT(product, productGlm);
            }
        }

        SECTION("Division")
        {
            {
                lm::FQuat quotient = big / 2.f;

                glm::quat quotientGlm = bigGlm / 2.f;

                CHECK_QUAT(quotient, quotientGlm);
            }
        }
    }

    SECTION("Transformation")
    {
        lm::FQuat const origin{ .5f, 2.f , 1.f, 2.5f };
        lm::FQuat const rotation{ 0.5f, 0.5f, 0.5f, 0.5f };

        glm::quat const originGlm{ 2.5f, .5f, 2.f, 1.f };
        glm::quat const rotationGlm{ 0.5f, 0.5f, 0.5f, 0.5f };

        SECTION("Rotate")
        {
            lm::FQuat rotated = rotation * origin;

            glm::quat rotatedGlm = rotationGlm * originGlm;

            CHECK_QUAT(rotated, rotatedGlm);
        }
    }

    SECTION("Functionality")
    {
        lm::FQuat const base{ .5f, 2.f, 7.f, 2.5f };
        lm::FQuat const other{ 3.f, 2.f, 1.f, 4.f };

        glm::quat const baseGlm{ 2.5f, .5f, 2.f, 7.f };
        glm::quat const otherGlm{ 4.f, 3.f, 2.f, 1.f };

        SECTION("Cross")
        {
            lm::FQuat cross = FQuat::Cross(base, other);

            glm::quat crossGlm = glm::cross(baseGlm, otherGlm);

            CHECK_QUAT(cross, crossGlm);
        }

        SECTION("Dot")
        {
            float dot = FQuat::Dot(base, other);

            float dotGlm = glm::dot(baseGlm, otherGlm);

            CHECK(dot == dotGlm);
        }

        SECTION("Length")
        {
            float length = FQuat::Length(base);

            float lengthGlm = glm::length(baseGlm);

            CHECK(length == lengthGlm);
        }

        SECTION("length2")
        {
            float lengthSquared = FQuat::Length2(base);

            float lengthSquaredGlm = glm::length(baseGlm) * glm::length(baseGlm);

            CHECK(lengthSquared == Catch::Approx(lengthSquaredGlm));
        }

        SECTION("Inverse")
        {
            lm::FQuat inverse = FQuat::Inverse(base);

            glm::quat inverseGlm = glm::inverse(baseGlm);

            CHECK_QUAT(inverse, inverseGlm);
        }

        SECTION("Normalize")
        {
            lm::FQuat normalized = FQuat::Normalize(base);

            glm::quat normalizedGlm = glm::normalize(baseGlm);

            CHECK_QUAT(normalized, normalizedGlm);
        }
    }

    SECTION("Extra")
    {
        SECTION("Serialization")
        {
            lm::FQuat const input{ 1.f, 2.f, 3.f, 4.f };

            std::stringstream buffer;
            buffer << input;
            CHECK(buffer.str() == "FQuat(1, 2, 3, 4)");
        }
    }
}