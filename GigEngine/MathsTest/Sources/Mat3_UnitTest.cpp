#include <Mat3/FMat3.hpp>

#define GLM_FORCE_SILENT_WARNINGS
#define GLM_FORCE_XYZW_ONLY
#include <glm/glm.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/projection.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/vector_angle.hpp>

#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>

using namespace lm;

#define CHECK_MAT3(mat, matGlm) CHECK(mat[0][0] == matGlm[0][0]); CHECK(mat[0][1] == matGlm[0][1]); CHECK(mat[0][2] == matGlm[0][2]); CHECK(mat[1][0] == matGlm[1][0]); CHECK(mat[1][1] == matGlm[1][1]); CHECK(mat[1][2] == matGlm[1][2]); CHECK(mat[2][0] == matGlm[2][0]); CHECK(mat[2][1] == matGlm[2][1]); CHECK(mat[2][2] == matGlm[2][2]);

TEST_CASE("Matrix3", "[.all][Matrix3][matrice]")
{
	SECTION("Instantiation")
	{
		// Default Constructor
		lm::FMat3 empty{};
		glm::mat3 emptyGlm{};

		CHECK_MAT3(empty, emptyGlm);

		// Destructor
		{
			lm::FMat3 temp;
		}

		// Basic Constructor
		lm::FMat3 allParam{ 1.f, 2.f, 3.f, 4.f, 5.f, 6.f, 7.f, 8.f, 9.f };
		glm::mat3 allParamGlm{ 1.f, 2.f, 3.f, 4.f, 5.f, 6.f, 7.f, 8.f, 9.f };
		CHECK_MAT3(allParam, allParamGlm);

		// Copy Constructor
		lm::FMat3 copy{ allParam };
		glm::mat3 copyGlm{ allParamGlm };
		CHECK_MAT3(copy, copyGlm);

		// Assignment Operator
		empty = allParam;
		emptyGlm = allParamGlm;
		CHECK_MAT3(empty, emptyGlm);

		// Size compare
		CHECK(sizeof lm::FMat3 == sizeof glm::mat3);

		// Memory
		CHECK(memcmp(&allParam, &allParamGlm, sizeof lm::FMat3) == 0);
	}

	SECTION("Accessor")
	{
		SECTION("Index Operator")
		{
			lm::FMat3 mat{ 1.f, 2.f, 3.f, 4.f, 5.f, 6.f, 7.f, 8.f, 9.f };
			glm::mat3 matGlm{ 1.f, 2.f, 3.f, 4.f, 5.f, 6.f, 7.f, 8.f, 9.f };

			CHECK_MAT3(mat, matGlm);

			mat[0][0] = 5.f;
			matGlm[0][0] = 5.f;
			CHECK_MAT3(mat, matGlm);

			mat[0][1] = 6.f;
			matGlm[0][1] = 6.f;
			CHECK_MAT3(mat, matGlm);

			mat[0][2] = 7.f;
		}
	}
	SECTION("Comparator")
	{
		SECTION("Equality")
		{
			lm::FMat3 mat{ 1.f, 2.f, 3.f, 4.f, 5.f, 6.f, 7.f, 8.f, 9.f };

			CHECK(mat == mat);
		}
		SECTION("Inequality")
		{
			lm::FMat3 mat{ 1.f, 2.f, 3.f, 4.f, 5.f, 6.f, 7.f, 8.f, 9.f };

			CHECK_FALSE(mat != mat);
		}
	}
	SECTION("Arithmetic")
	{
		SECTION("Addition")
		{
			lm::FMat3 mat{ 1.f, 2.f, 3.f, 4.f, 5.f, 6.f, 7.f, 8.f, 9.f };
			glm::mat3 matGlm{ 1.f, 2.f, 3.f, 4.f, 5.f, 6.f, 7.f, 8.f, 9.f };

			mat += mat;
			matGlm += matGlm;
			CHECK_MAT3(mat, matGlm);

			mat = mat + mat;
			matGlm = matGlm + matGlm;
			CHECK_MAT3(mat, matGlm);
		}
		SECTION("Subtraction")
		{
			lm::FMat3 mat{ 1.f, 2.f, 3.f, 4.f, 5.f, 6.f, 7.f, 8.f, 9.f };
			glm::mat3 matGlm{ 1.f, 2.f, 3.f, 4.f, 5.f, 6.f, 7.f, 8.f, 9.f };

			mat -= mat;
			matGlm -= matGlm;
			CHECK_MAT3(mat, matGlm);

			mat = mat - mat;
			matGlm = matGlm - matGlm;
			CHECK_MAT3(mat, matGlm);
		}
		SECTION("Multiplication")
		{
			lm::FMat3 mat{ 1.f, 2.f, 3.f, 4.f, 5.f, 6.f, 7.f, 8.f, 9.f };

			glm::mat3 matGlm{ 1.f, 2.f, 3.f, 4.f, 5.f, 6.f, 7.f, 8.f, 9.f };

			mat *= mat;
			matGlm *= matGlm;
			CHECK_MAT3(mat, matGlm);

			mat = mat * mat;
			matGlm = matGlm * matGlm;
			CHECK_MAT3(mat, matGlm);
		}
	}
	SECTION("Functionnality")
	{
		SECTION("Identity")
		{
			lm::FMat3 mat = lm::FMat3::Identity();
			glm::mat3 matGlm = glm::mat3(1.f);

			CHECK_MAT3(mat, matGlm);
		}
		SECTION("Transpose")
		{
			lm::FMat3 mat{ 1.f, 2.f, 3.f, 4.f, 5.f, 6.f, 7.f, 8.f, 9.f };
			glm::mat3 matGlm{ 1.f, 2.f, 3.f, 4.f, 5.f, 6.f, 7.f, 8.f, 9.f };

			mat = FMat3::Transpose(mat);
			matGlm = glm::transpose(matGlm);
			CHECK_MAT3(mat, matGlm);
		}
		SECTION("Inverse")
		{
			lm::FMat3 mat{ 1.f, 2.f, 3.f, 4.f, 5.f, 6.f, 7.f, 8.f, 9.f };
			glm::mat3 matGlm{ 1.f, 2.f, 3.f, 4.f, 5.f, 6.f, 7.f, 8.f, 9.f };
			mat = FMat3::Inverse(mat);
			matGlm = glm::inverse(matGlm);
			CHECK_MAT3(mat, matGlm);
		}
		SECTION("Identity")
		{
			lm::FMat3 mat = lm::FMat3::Identity();
			glm::mat3 matGlm = glm::mat3(1.f);

			CHECK_MAT3(mat, matGlm);
		}
		SECTION("Scale")
		{
			lm::FMat3 mat = lm::FMat3::Scale(FVec3(1, 2, 3));
			glm::mat3 matGlm = glm::scale(glm::vec3(1, 2, 3));

			CHECK_MAT3(mat, matGlm);
		}
		SECTION("Rotate")
		{
			lm::FMat3 mat = lm::FMat3::Rotate(FMat3::Identity(), 45.f, lm::FVec3(1, 2, 3));
			glm::mat3 matGlm = glm::rotate(45.f, glm::vec3(1, 2, 3));

			CHECK_MAT3(mat, matGlm);
		}
	}
}