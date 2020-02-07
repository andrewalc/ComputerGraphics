// Includes for the assignment
#include "Vector4f.h"
#include "Matrix4f.h"
#include <iostream>

// Tests for comparing our library
// You may compare your operations against the glm library
// which is a well tested library.
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Sample unit test comparing against GLM.
bool unitTest0(){
	glm::mat4 glmIdentityMatrix = glm::mat4(1.0f);
	Matrix4f myIdentity( 1.0f,0,0,0,
        			     0,1.0f,0,0,
		        	     0,0,1.0f,0,
			             0,0,0,1.0f);

    if(
        glmIdentityMatrix[0][0]==myIdentity[0][0] &&
        glmIdentityMatrix[0][1]==myIdentity[0][1] &&
        glmIdentityMatrix[0][2]==myIdentity[0][2] &&
        glmIdentityMatrix[0][3]==myIdentity[0][3] &&
        glmIdentityMatrix[1][0]==myIdentity[1][0] &&
        glmIdentityMatrix[1][1]==myIdentity[1][1] &&
        glmIdentityMatrix[1][2]==myIdentity[1][2] &&
        glmIdentityMatrix[1][3]==myIdentity[1][3] &&
        glmIdentityMatrix[2][0]==myIdentity[2][0] &&
        glmIdentityMatrix[2][1]==myIdentity[2][1] &&
        glmIdentityMatrix[2][2]==myIdentity[2][2] &&
        glmIdentityMatrix[2][3]==myIdentity[2][3] &&
        glmIdentityMatrix[3][0]==myIdentity[3][0] &&
        glmIdentityMatrix[3][1]==myIdentity[3][1] &&
        glmIdentityMatrix[3][2]==myIdentity[3][2] &&
        glmIdentityMatrix[3][3]==myIdentity[3][3]){
            return true;
    }
	
    return false;	
}

bool unitTest1(){
	glm::mat4 glmIdentityMatrix = glm::mat4(1.0f);
	Matrix4f myIdentity( 1.0f,0,0,0,
        			     0,1.0f,0,0,
		        	     0,0,1.0f,0,
			             0,0,0,1.0f);

    if(
        glmIdentityMatrix[0][0]==myIdentity(0,0) &&
        glmIdentityMatrix[0][1]==myIdentity(0,1) &&
        glmIdentityMatrix[0][2]==myIdentity(0,2) &&
        glmIdentityMatrix[0][3]==myIdentity(0,3) &&
        glmIdentityMatrix[1][0]==myIdentity(1,0) &&
        glmIdentityMatrix[1][1]==myIdentity(1,1) &&
        glmIdentityMatrix[1][2]==myIdentity(1,2) &&
        glmIdentityMatrix[1][3]==myIdentity(1,3) &&
        glmIdentityMatrix[2][0]==myIdentity(2,0) &&
        glmIdentityMatrix[2][1]==myIdentity(2,1) &&
        glmIdentityMatrix[2][2]==myIdentity(2,2) &&
        glmIdentityMatrix[2][3]==myIdentity(2,3) &&
        glmIdentityMatrix[3][0]==myIdentity(3,0) &&
        glmIdentityMatrix[3][1]==myIdentity(3,1) &&
        glmIdentityMatrix[3][2]==myIdentity(3,2) &&
        glmIdentityMatrix[3][3]==myIdentity(3,3)){
            return true;
    }
	
    return false;	
}

// Sample unit test comparing against GLM.
bool unitTest2(){
	glm::mat4 glmIdentityMatrix = glm::mat4(1.0f);
	Vector4f a(1.0f ,0.0f,  0.0f,   0.0f);
	Vector4f b(0.0f ,1.0f,  0.0f,   0.0f);
	Vector4f c(0.0f ,0.0f,  1.0f,   0.0f);
	Vector4f d(0.0f ,0.0f,  0.0f,   1.0f);
	Matrix4f myIdentity(a,b,c,d);

    if(
        glmIdentityMatrix[0][0]==myIdentity[0][0] &&
        glmIdentityMatrix[0][1]==myIdentity[0][1] &&
        glmIdentityMatrix[0][2]==myIdentity[0][2] &&
        glmIdentityMatrix[0][3]==myIdentity[0][3] &&
        glmIdentityMatrix[1][0]==myIdentity[1][0] &&
        glmIdentityMatrix[1][1]==myIdentity[1][1] &&
        glmIdentityMatrix[1][2]==myIdentity[1][2] &&
        glmIdentityMatrix[1][3]==myIdentity[1][3] &&
        glmIdentityMatrix[2][0]==myIdentity[2][0] &&
        glmIdentityMatrix[2][1]==myIdentity[2][1] &&
        glmIdentityMatrix[2][2]==myIdentity[2][2] &&
        glmIdentityMatrix[2][3]==myIdentity[2][3] &&
        glmIdentityMatrix[3][0]==myIdentity[3][0] &&
        glmIdentityMatrix[3][1]==myIdentity[3][1] &&
        glmIdentityMatrix[3][2]==myIdentity[3][2] &&
        glmIdentityMatrix[3][3]==myIdentity[3][3]){
            return true;
    }
	
    return false;	
}

// Sample unit test comparing against GLM.
// TODO: Test against glm::scale
bool unitTest3(){
	glm::mat4 glmScale = glm::mat4(2.0f);
	glmScale[3][3] = 1; //changed to 1 because w should always be 1, and Piazza post suggested this
	Vector4f a(1.0f,0,0,0);
	Vector4f b(0.0f,1.0f,0,0);
	Vector4f c(0,0,1.0f,0);
	Vector4f d(0,0,0,1.0f);
	Matrix4f myScaled(a,b,c,d);
    Matrix4f newScale = myScaled.MakeScale(2.0f,2.0f,2.0f);

    if(
        glmScale[0][0]==newScale[0][0] &&
        glmScale[0][1]==newScale[0][1] &&
        glmScale[0][2]==newScale[0][2] &&
        glmScale[0][3]==newScale[0][3] &&
        glmScale[1][0]==newScale[1][0] &&
        glmScale[1][1]==newScale[1][1] &&
        glmScale[1][2]==newScale[1][2] &&
        glmScale[1][3]==newScale[1][3] &&
        glmScale[2][0]==newScale[2][0] &&
        glmScale[2][1]==newScale[2][1] &&
        glmScale[2][2]==newScale[2][2] &&
        glmScale[2][3]==newScale[2][3] &&
        glmScale[3][0]==newScale[3][0] &&
        glmScale[3][1]==newScale[3][1] &&
        glmScale[3][2]==newScale[3][2] &&
        glmScale[3][3]==newScale[3][3]){
            return true;
    }
	
    return false;	
}

// Sample unit test comparing against GLM.
// Testing operator
bool unitTest4(){
	glm::mat4 glmTest = glm::mat4(1.0f);
    glmTest[1][3] = 72.0f;
    glmTest[2][3] = 2.1f;

	Matrix4f myMatrix(0,0,0,0,
                      0,0,0,0,
                      0,0,0,0,
                      0,0,0,0);

    myMatrix[1][3] = 72.0f;
    myMatrix[2][3] = 2.1f;

    if( glmTest[1][3]==myMatrix[1][3] &&
        glmTest[2][3]==myMatrix[2][3] ){
            return true;
    }
	
    return false;	
}

// Sample unit test testing your library
bool unitTest5(){
  Vector4f a(1,1,1,1);
  Vector4f b(0,0,0,0);
  Vector4f c = a + b;

  if(c.x == 1 && c.y == 1 && c.z ==1 && c.w==1){
    return true;
  }
    return false;
}

//test *= operator
bool unitTest6() {
	Vector4f a(1, 1, 1, 1);
	float s = 2;

	a *= s;

	if (a.x == 2 && a.y == 2 && a.z == 2 && a.w == 2) {
		return true;
	}
	return false;
}

//test dot product
bool unitTest7() {
	Vector4f a(1, 2, 3, 1);
	Vector4f b(2, 3, 4, 1);

	float result = Dot(a, b);

	if (result == 21) {
		return true;
	}
	return false;
}

// test negative operator
bool unitTest8() {
	Vector4f a(1, 2, 3, 1);
	Vector4f newA = -a;

	if (newA.x == -1 && newA.y == -2 && newA.z == -3 && newA.w == -1) {
		return true;
	}
	return false;
}

// test Magnitude
bool unitTest9() {
	Vector4f a(1, 1, 1, 1);
	float newA = Magnitude(a);

	if (newA == 2) {
		return true;
	}
	return false;
}


// test Magnitude
bool unitTest10() {
	Vector4f a(2.0f, 0, 0, 0);
	Vector4f b(0.0f, 3.0f, 0, 0);
	Vector4f c(0, 0, 4.0f, 0);
	Vector4f d(0, 0, 0, 1.0f);
	Matrix4f matA = Matrix4f(a, b, c, d);

	Vector4f e(1.0f, 0, 0, 0);
	Vector4f f(0.0f, 2.0f, 0, 0);
	Vector4f g(0, 0, 3.0f, 0);
	Vector4f h(0, 0, 0, 1.0f);
	Matrix4f matB= Matrix4f(e, f, g, h);

	Matrix4f result = matA * matB;

	Vector4f i(2.0f, 0, 0, 0);
	Vector4f j(0.0f, 6.0f, 0, 0);
	Vector4f k(0, 0, 12.0f, 0);
	Vector4f l(0, 0, 0, 1.0f);
	Matrix4f matC = Matrix4f(i, j, k, l);

	
	if (result[0][0] == matC[0][0] &&
		result[0][1] == matC[0][1] &&
		result[0][2] == matC[0][2] &&
		result[0][3] == matC[0][3] &&
		result[1][0] == matC[1][0] &&
		result[1][1] == matC[1][1] &&
		result[1][2] == matC[1][2] &&
		result[1][3] == matC[1][3] &&
		result[2][0] == matC[2][0] &&
		result[2][1] == matC[2][1] &&
		result[2][2] == matC[2][2] &&
		result[2][3] == matC[2][3] &&
		result[3][0] == matC[3][0] &&
		result[3][1] == matC[3][1] &&
		result[3][2] == matC[3][2] &&
		result[3][3] == matC[3][3]) {
			return true;
	}
	return false;
}




int main(){
    // Keep track of the tests passed
    unsigned int testsPassed = 0;

    // Run 'unit tests'
    std::cout << "Passed 0: " << unitTest0() << " \n";
    std::cout << "Passed 1: " << unitTest1() << " \n";
    std::cout << "Passed 2: " << unitTest2() << " \n";
    std::cout << "Passed 3: " << unitTest3() << " \n";
    std::cout << "Passed 4: " << unitTest4() << " \n";
    std::cout << "Passed 5: " << unitTest5() << " \n";
	std::cout << "Passed 6: " << unitTest6() << " \n";
	std::cout << "Passed 7: " << unitTest7() << " \n";
	std::cout << "Passed 8: " << unitTest8() << " \n";
	std::cout << "Passed 9: " << unitTest9() << " \n";
	std::cout << "Passed 10: " << unitTest10() << " \n";

    return 0;
}
