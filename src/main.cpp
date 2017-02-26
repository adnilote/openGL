
#include <iostream>
#include <vector>
#include <string>
#include "Utility.h"

using namespace std;

const uint GRASS_INSTANCES = 10000; // Количество травинок

GL::Camera camera;               // Мы предоставляем Вам реализацию камеры. В OpenGL камера - это просто 2 матрицы. Модельно-видовая матрица и матрица проекции. // ###
                                 // Задача этого класса только в том чтобы обработать ввод с клавиатуры и правильно сформировать эти матрицы.
                                 // Вы можете просто пользоваться этим классом для расчёта указанных матриц.


GLuint grassPointsCount; // Количество вершин у модели травинки
GLuint grassShader;      // Шейдер, рисующий траву
GLuint grassVAO;         // VAO для травы (что такое VAO почитайте в доках)
GLuint grassVariance;    // Буфер для смещения координат травинок
vector<VM::vec4> grassVarianceData(GRASS_INSTANCES); // Вектор со смещениями для координат травинок
vector<VM::vec4> grassV(GRASS_INSTANCES); // скорость
vector<VM::vec4> grassA(GRASS_INSTANCES); // ускорение
GLuint grasstexture[2];
GLuint grassTBO = -1;//// Это "фиктивная" текстура для чтения из буфера. На самом деле никакой текстуры не создаётся.
GLuint grassTBO2 = -1;// аналогично

const uint Clover_INSTANCES = 225;
GLuint cloverPointsCount;
vector<VM::vec2> cloverPos(Clover_INSTANCES);
GLuint cloverShader;      // Шейдер, рисующий траву
GLuint cloverVAO;         // VAO для травы (что такое VAO почитайте в доках)
GLuint cloverVariance;    // Буфер для смещения координат травинок
vector<VM::vec4> cloverVarianceData(GRASS_INSTANCES); // Вектор со смещениями для координат травинок
vector<VM::vec4> cloverV(GRASS_INSTANCES); // скорость
vector<VM::vec4> cloverA(GRASS_INSTANCES); // ускорение
GLuint clovertexture;
GLuint cloverTBO = -1;//// Это "фиктивная" текстура для чтения из буфера. На самом деле никакой текстуры не создаётся.
GLuint cloverTBO2 = -1;// аналогично

const uint plant_INSTANCES = 100;
GLuint plantPointsCount;
vector<VM::vec2> plantPos(plant_INSTANCES);
GLuint plantShader;      // Шейдер, рисующий траву
GLuint plantVAO;         // VAO для травы (что такое VAO почитайте в доках)
GLuint plantVariance;    // Буфер для смещения координат травинок
vector<VM::vec4> plantVarianceData(GRASS_INSTANCES); // Вектор со смещениями для координат травинок
vector<VM::vec4> plantV(GRASS_INSTANCES); // скорость
vector<VM::vec4> plantA(GRASS_INSTANCES); // ускорение
GLuint planttexture;
GLuint plantTBO = -1;//// Это "фиктивная" текстура для чтения из буфера. На самом деле никакой текстуры не создаётся.
GLuint plantTBO2 = -1;// аналогично

GLuint groundShader; // Шейдер для земли
GLuint groundVAO;    // VAO для земли
GLuint groundtexture;

GLuint mountShader;
GLuint mountVAO;
GLuint mountPointsCount;
GLuint mountUV;
GLuint mounttexture;

GLuint skyShader;
GLuint skyVAO;
GLuint skyPointsCount;
GLuint skyUV;
GLuint skytexture;

GLuint waterShader;
GLuint waterVAO;
GLuint waterPointsCount;
GLuint waterUV;
GLuint watertexture;

GLuint butShader;
GLuint butVAO;
GLuint butPointsCount;
GLuint butUV;
GLuint buttexture;

GLuint horseShader;
GLuint horseVAO;
GLuint horsePointsCount;
GLuint horseUV;
GLuint horsetexture;

GLuint houseShader;
GLuint houseVAO;
GLuint housePointsCount;
GLuint houseUV;
GLuint housetexture;

GLuint treeShader;
GLuint treeVAO;
GLuint treePointsCount;
GLuint treeUV;
GLuint treetexture;

GLuint streeShader;
GLuint streeVAO;
GLuint streePointsCount;
GLuint streeUV;
GLuint streetexture;

// Размеры экрана
uint screenWidth = 800;
uint screenHeight = 600;


GLuint cam;
// Это для захвата мышки. Вам это не потребуется
bool captureMouse = true;


// Функция, для привязки к шейдеру TBO - буфера через текстуру
void bindTextureBuffer(GLuint program, GLint unit, const GLchar *name, GLuint texture) {

  glActiveTexture(GL_TEXTURE0 + unit);       CHECK_GL_ERRORS; // делаем активным определённый текстурныю юнит/блок
  glBindTexture(GL_TEXTURE_BUFFER, texture); CHECK_GL_ERRORS; // делаем текущей текстуру отвечающую за наш буфер

  GLint location = glGetUniformLocation(program, name);   CHECK_GL_ERRORS;  // привязываем текущую текстуру (которая фикктивная) и теущим текстурным блоком к имени в шейдере
  if (location >= 0)                                                        // на следующей строчке
    glUniform1i(location, unit);                                            // да да вот тут :)
}

//Ф-ция загрузки obj файла 2 видов, подсмотрено в инете, но доработано
bool loadOBJ(const char * path, std::vector < VM::vec3 > & out_vertices,	std::vector < VM::vec2 > & out_uvs,	std::vector < VM::vec3 > & out_normals) 
{
	std::vector< unsigned int > vertexIndices, uvIndices, normalIndices;
	std::vector< VM::vec3 > temp_vertices;
	std::vector< VM::vec2 > temp_uvs;
	std::vector< VM::vec3 > temp_normals;

	FILE * file = fopen(path, "r");
	if (file == NULL) {
		printf("Impossible to open the file !\n");    
		return false;
	}

	while (1) {
		char lineHeader[128];
		// читаем первый символ в файле
		int res = fscanf(file, "%s", lineHeader);
		if (res == EOF)
			break; // EOF = Конец файла. Заканчиваем цикл чтения
				   // else : парсим строку
		if (strcmp(lineHeader, "v") == 0) {
			VM::vec3 vertex;
			fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
			temp_vertices.push_back(vertex);
		}
		else if (strcmp(lineHeader, "vt") == 0) {
			VM::vec2 uv;
			fscanf(file, "%f %f\n", &uv.x, &uv.y);
			temp_uvs.push_back(uv);
		}
		else if (strcmp(lineHeader, "vn") == 0) {
			VM::vec3 normal;
			fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
			temp_normals.push_back(normal);
		}
		else if (strcmp(lineHeader, "f") == 0) 
		{
			std::string vertex1, vertex2, vertex3;
			unsigned int vertexIndex[4], uvIndex[4], normalIndex[4];
			int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2], &vertexIndex[3], &uvIndex[3], &normalIndex[3]);
			if (matches == 9)
			{
				vertexIndices.push_back(vertexIndex[0]);
				vertexIndices.push_back(vertexIndex[1]);
				vertexIndices.push_back(vertexIndex[2]);
				uvIndices.push_back(uvIndex[0]);
				uvIndices.push_back(uvIndex[1]);
				uvIndices.push_back(uvIndex[2]);
				normalIndices.push_back(normalIndex[0]);
				normalIndices.push_back(normalIndex[1]);
				normalIndices.push_back(normalIndex[2]);
			}
			else
				if (matches == 12)
				{
					vertexIndices.push_back(vertexIndex[0]);
					vertexIndices.push_back(vertexIndex[1]);
					vertexIndices.push_back(vertexIndex[3]);
					vertexIndices.push_back(vertexIndex[1]);
					vertexIndices.push_back(vertexIndex[2]);
					vertexIndices.push_back(vertexIndex[3]);

					uvIndices.push_back(uvIndex[0]);
					uvIndices.push_back(uvIndex[1]);
					uvIndices.push_back(uvIndex[3]);
					uvIndices.push_back(uvIndex[1]);
					uvIndices.push_back(uvIndex[2]);
					uvIndices.push_back(uvIndex[3]);

					normalIndices.push_back(normalIndex[0]);
					normalIndices.push_back(normalIndex[1]);
					normalIndices.push_back(normalIndex[3]);
					normalIndices.push_back(normalIndex[1]);
					normalIndices.push_back(normalIndex[2]);
					normalIndices.push_back(normalIndex[3]);

				}
			else
				{
				printf("File can't be read by our simple parser \n");
				return false;
				}
		}
	}
				// По каждой вершине каждого треугольника
			for (unsigned int i = 0; i < vertexIndices.size(); i++) {

				unsigned int vertexIndex = vertexIndices[i];

				VM::vec3 vertex = temp_vertices[vertexIndex - 1];

				out_vertices.push_back(vertex);
			}

			for (unsigned int i = 0; i < uvIndices.size(); i++) {

				unsigned int uvIndex = uvIndices[i];

				VM::vec2 vertex = temp_uvs[uvIndex - 1];	

				out_uvs.push_back(vertex);
			}/*
			for (unsigned int i = 0; i < normalIndices.size(); i++) {

				unsigned int vertexIndex = normalIndices[i];

				VM::vec3 vertex = temp_normals[vertexIndex - 1];

				out_normals.push_back(vertex);
			}*/
			return true;
}

// Функция, рисующая землю
void DrawGround() {
    // Используем шейдер для земли
	glUseProgram(groundShader);                                                  CHECK_GL_ERRORS

	glActiveTexture(GL_TEXTURE0 + 8);

	glBindTexture(GL_TEXTURE_2D, groundtexture);
	//GLuint loc = glGetUniformLocation(groundShader, "myTexture");
	glUniform1i(glGetUniformLocation(groundShader, "myTexture"), 8);


    // Устанавливаем юниформ для шейдера. В данном случае передадим перспективную матрицу камеры
    // Находим локацию юниформа 'camera' в шейдере
    GLint cameraLocation = glGetUniformLocation(groundShader, "camera");         CHECK_GL_ERRORS
    // Устанавливаем юниформ (загружаем на GPU матрицу проекции?)                                                     // ###
    glUniformMatrix4fv(cameraLocation, 1, GL_TRUE, camera.getMatrix().data().data()); CHECK_GL_ERRORS

    // Подключаем VAO, который содержит буферы, необходимые для отрисовки земли
    glBindVertexArray(groundVAO);                                                CHECK_GL_ERRORS

    // Рисуем землю: 2 треугольника (6 вершин)
    glDrawArrays(GL_TRIANGLES, 0, 6);                                            CHECK_GL_ERRORS

    // Отсоединяем VAO
    glBindVertexArray(0);                                                        CHECK_GL_ERRORS
    // Отключаем шейдер
    glUseProgram(0);                                                             CHECK_GL_ERRORS
}

//ф-ция для рисования любых статичных обьектов 
void DrawObject(string name, 
	GLuint objShader, int uint, GLuint obj_texture, const char* texname_in_sh, GLuint objVAO, GLuint obj_pointcount) {
	// Используем шейдер для земли
	glUseProgram(objShader);                                                  CHECK_GL_ERRORS

	
	glActiveTexture(GL_TEXTURE0 + uint);
	glBindTexture(GL_TEXTURE_2D, obj_texture);
	glUniform1i(glGetUniformLocation(objShader, texname_in_sh), uint);


	// Устанавливаем юниформ для шейдера. В данном случае передадим перспективную матрицу камеры
	// Находим локацию юниформа 'camera' в шейдере
	GLint cameraLocation = glGetUniformLocation(objShader, "camera");         CHECK_GL_ERRORS
		// Устанавливаем юниформ (загружаем на GPU матрицу проекции?)                                                     // ###
		glUniformMatrix4fv(cameraLocation, 1, GL_TRUE, camera.getMatrix().data().data()); CHECK_GL_ERRORS
		// Подключаем VAO, который содержит буферы, необходимые для отрисовки земли
		glBindVertexArray(objVAO);                                                CHECK_GL_ERRORS

	glDrawArrays(GL_TRIANGLES, 0, obj_pointcount);                                            CHECK_GL_ERRORS

		

	// Отсоединяем VAO
	glBindVertexArray(0);                                                        CHECK_GL_ERRORS
	// Отключаем шейдер
	glUseProgram(0);                                                             CHECK_GL_ERRORS
}

//использует кубич текстуры
void DrawSkybox(
	GLuint objShader, int uint, GLuint obj_texture, const char* texname_in_sh, GLuint objVAO, GLuint obj_pointcount) {
	// Используем шейдер для земли

	//glDepthMask(GL_FALSE); CHECK_GL_ERRORS
	glUseProgram(objShader);                                                  CHECK_GL_ERRORS

	glActiveTexture(GL_TEXTURE0 + uint);
	glBindTexture(GL_TEXTURE_CUBE_MAP, obj_texture);
	glUniform1i(glGetUniformLocation(objShader, texname_in_sh), uint);

	GLint cameraLocation = glGetUniformLocation(objShader, "camera");         CHECK_GL_ERRORS
		glUniformMatrix4fv(cameraLocation, 1, GL_TRUE, camera.getMatrix().data().data()); CHECK_GL_ERRORS
		glBindVertexArray(objVAO);                                                CHECK_GL_ERRORS


		//	GLint cameraLocation = glGetUniformLocation(objShader, "skymove");          CHECK_GL_ERRORS
		//	glUniform3f(cameraLocation, camera.position.z, camera.position.z, camera.position.z); CHECK_GL_ERRORS
		//	glBindVertexArray(objVAO);     CHECK_GL_ERRORS
		//
	//bindTextureBuffer(skyShader, 4, "skymove", cam);// привязываем TBO к имени в шейдере

	glDrawArrays(GL_TRIANGLES, 0, obj_pointcount);                                            CHECK_GL_ERRORS
		glBindVertexArray(0);                                                        CHECK_GL_ERRORS
		glUseProgram(0);                                                             CHECK_GL_ERRORS
		//glDepthMask(GL_TRUE); CHECK_GL_ERRORS
}

// Функция, рисующая гору
void DrawMount() {
	// Используем шейдер для земли
	glUseProgram(mountShader);                                                  CHECK_GL_ERRORS

	glActiveTexture(GL_TEXTURE0 + 7);
	glBindTexture(GL_TEXTURE_2D, mounttexture);
	//GLuint loc = glGetUniformLocation(groundShader, "myTexture");
	glUniform1i(glGetUniformLocation(mountShader, "mountTexture"), 7);


	// Устанавливаем юниформ для шейдера. В данном случае передадим перспективную матрицу камеры
	// Находим локацию юниформа 'camera' в шейдере
	GLint cameraLocation = glGetUniformLocation(mountShader, "camera");         CHECK_GL_ERRORS
		// Устанавливаем юниформ (загружаем на GPU матрицу проекции?)                                                     // ###
		glUniformMatrix4fv(cameraLocation, 1, GL_TRUE, camera.getMatrix().data().data()); CHECK_GL_ERRORS

		// Подключаем VAO, который содержит буферы, необходимые для отрисовки земли
		glBindVertexArray(mountVAO);                                                CHECK_GL_ERRORS

		///////////////////////////////////////// Рисуем землю: 2 треугольника (9 вершин)
		glDrawArrays(GL_TRIANGLES, 0, 9);                                            CHECK_GL_ERRORS

		// Отсоединяем VAO
		glBindVertexArray(0);                                                        CHECK_GL_ERRORS
		// Отключаем шейдер
		glUseProgram(0);                                                             CHECK_GL_ERRORS
}

float time = 0.05f;
// Обновление смещения травинок
void UpdateGrassVariance() {
    // Генерация случайных смещений
	for (uint i = 0; i < GRASS_INSTANCES; ++i) {

		grassA[i].x = 1.5f/* *rand()/(RAND_MAX)*/ - 0.1f*grassV[i].x - grassVarianceData[i].x*10; // a=F-bv-k*x0
			grassV[i].x += grassA[i].x*time; // v = v0 + at
			grassVarianceData[i].x += grassV[i].x*time; // x=x0 + vt
	
			grassA[i].z = 1.5f - grassVarianceData[i].z*10.0; // a=F-k*x0
			grassV[i].z += grassA[i].z*time; // v = v0 + at
			grassVarianceData[i].z += grassV[i].z*time; // x=x0 + vt

			grassVarianceData[i].y = 
			abs(1.0f - sqrt(1.0f-grassVarianceData[i].x*grassVarianceData[i].x));
			
			cloverVarianceData[i].y = plantVarianceData[i].y = grassVarianceData[i].y;
			
	}

    // Привязываем буфер, содержащий смещения
    glBindBuffer(GL_ARRAY_BUFFER, grassVariance);                                CHECK_GL_ERRORS
    // Загружаем данные в видеопамять
    glBufferData(GL_ARRAY_BUFFER, sizeof(VM::vec4) * GRASS_INSTANCES, grassVarianceData.data(), GL_STATIC_DRAW); CHECK_GL_ERRORS
    // Отвязываем буфер
    glBindBuffer(GL_ARRAY_BUFFER, 0);  CHECK_GL_ERRORS


		// Привязываем буфер, содержащий смещения
		glBindBuffer(GL_ARRAY_BUFFER, cloverVariance);                                CHECK_GL_ERRORS
		// Загружаем данные в видеопамять
		glBufferData(GL_ARRAY_BUFFER, sizeof(VM::vec4) * Clover_INSTANCES, cloverVarianceData.data(), GL_STATIC_DRAW); CHECK_GL_ERRORS
		// Отвязываем буфер
		glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Привязываем буфер, содержащий смещения
	glBindBuffer(GL_ARRAY_BUFFER, plantVariance);                                CHECK_GL_ERRORS
		// Загружаем данные в видеопамять
		glBufferData(GL_ARRAY_BUFFER, sizeof(VM::vec4) * plant_INSTANCES, plantVarianceData.data(), GL_STATIC_DRAW); CHECK_GL_ERRORS
		// Отвязываем буфер
		glBindBuffer(GL_ARRAY_BUFFER, 0);
}
void DrawGrass() {
    // Тут то же самое, что и в рисовании земли
	glUseProgram(grassShader);                  CHECK_GL_ERRORS

	glActiveTexture(GL_TEXTURE0 + 4);
	glBindTexture(GL_TEXTURE_2D, grasstexture[0]);
	glUniform1i(glGetUniformLocation(grassShader, "GrassTex"), 4);

	glActiveTexture(GL_TEXTURE0 + 14);
	glBindTexture(GL_TEXTURE_2D, grasstexture[1]);
	glUniform1i(glGetUniformLocation(grassShader, "DirtTex"), 14);

	GLint cameraLocation = glGetUniformLocation(grassShader, "camera");          CHECK_GL_ERRORS
		glUniformMatrix4fv(cameraLocation, 1, GL_TRUE, camera.getMatrix().data().data()); CHECK_GL_ERRORS
		glBindVertexArray(grassVAO);     CHECK_GL_ERRORS

		// Обновляем смещения для травы

    UpdateGrassVariance();
    // Отрисовка травинок в количестве GRASS_INSTANCES

    bindTextureBuffer(grassShader, 2, "tboSampler",  grassTBO); // привязываем TBO к имени в шейдере
    bindTextureBuffer(grassShader, 3, "tboSampler2", grassTBO2);// привязываем TBO к имени в шейдере
	
	glDrawArraysInstanced(GL_TRIANGLES, 0, grassPointsCount, GRASS_INSTANCES);   CHECK_GL_ERRORS
    glBindVertexArray(0);                                                        CHECK_GL_ERRORS
    glUseProgram(0);                                                             CHECK_GL_ERRORS
}

//для рисования обьектов, колышашие на ветру
void DrawPlant(/*GLuint plaShader, int uint, GLuint pla_tex, const char* name_insh,  GLuint plaVAO, GLuint pla_pointcount*/) {

	// Тут то же самое, что и в рисовании земли
	glUseProgram(plantShader);                  CHECK_GL_ERRORS

		glActiveTexture(GL_TEXTURE0 + 11);
	glBindTexture(GL_TEXTURE_2D, planttexture);
	glUniform1i(glGetUniformLocation(plantShader, "Tex"), 11);

	GLint cameraLocation = glGetUniformLocation(plantShader, "camera");          CHECK_GL_ERRORS
		glUniformMatrix4fv(cameraLocation, 1, GL_TRUE, camera.getMatrix().data().data()); CHECK_GL_ERRORS
		glBindVertexArray(plantVAO);     CHECK_GL_ERRORS

	bindTextureBuffer(plantShader, 13, "tboSampler",  plantTBO); // привязываем TBO к имени в шейдере
	bindTextureBuffer(plantShader, 14, "tboSampler2", plantTBO2);// привязываем TBO к имени в шейдере

	glDrawArraysInstanced(GL_TRIANGLES, 0, plantPointsCount, plant_INSTANCES);   CHECK_GL_ERRORS
		glBindVertexArray(0);                                                        CHECK_GL_ERRORS
		glUseProgram(0);                                             
                CHECK_GL_ERRORS
}

void DrawClover(){
	glUseProgram(cloverShader);                                                  CHECK_GL_ERRORS

	glActiveTexture(GL_TEXTURE0 + 6);

	glBindTexture(GL_TEXTURE_2D, clovertexture);
	glUniform1i(glGetUniformLocation(cloverShader, "cloverTexture"), 6);

	GLint cameraLocation = glGetUniformLocation(cloverShader, "camera");         CHECK_GL_ERRORS
	glUniformMatrix4fv(cameraLocation, 1, GL_TRUE, camera.getMatrix().data().data()); CHECK_GL_ERRORS

	glBindVertexArray(cloverVAO);                                                CHECK_GL_ERRORS

	//UpdateCloverVariance(); CHECK_GL_ERRORS

	bindTextureBuffer(cloverShader, 2, "tboSamplerC",  cloverTBO); // привязываем TBO к имени в шейдере
	bindTextureBuffer(cloverShader, 3, "tboSamplerC2", cloverTBO2);// привязываем TBO к имени в шейдере

		glDrawArraysInstanced(GL_TRIANGLES, 0, cloverPointsCount, Clover_INSTANCES);   CHECK_GL_ERRORS
		// Отсоединяем VAO
		glBindVertexArray(0);                                                        CHECK_GL_ERRORS
		// Отключаем шейдер
		glUseProgram(0);                                                             CHECK_GL_ERRORS
}

// Эта функция вызывается для обновления экрана
void RenderLayouts() {
    // Включение буфера глубины
    glEnable(GL_DEPTH_TEST);
    // Очистка буфера глубины и цветового буфера
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // Рисуем меш
	DrawMount();

	DrawGrass();	
	DrawClover();
 	DrawPlant();

	DrawObject("horse", horseShader, 10, horsetexture, "Texture", horseVAO, horsePointsCount);
	DrawObject("house", houseShader, 10, housetexture, "Texture", houseVAO, housePointsCount);
	
	DrawGround();
	DrawObject("water", waterShader, 13, watertexture, "Texture", waterVAO, waterPointsCount);

	DrawObject("tree", treeShader, 3, treetexture, "Texture", treeVAO, treePointsCount);
	//DrawObject("tree", streeShader, 4, streetexture, "Texture", streeVAO, streePointsCount);
	DrawObject("but", butShader, 7, buttexture, "Texture", butVAO, butPointsCount);
	
	DrawSkybox( skyShader, 19, skytexture, "skyTexture", skyVAO, skyPointsCount);

    glutSwapBuffers();
}

// Завершение программы
void FinishProgram() {
    glutDestroyWindow(glutGetWindow());
}

int msaa = 1;
// Обработка события нажатия клавиши (специальные клавиши обрабатываются в функции SpecialButtons)
void KeyboardEvents(unsigned char key, int x, int y) {
    if (key == 27) {
        FinishProgram();
    } else if (key == 'w') {
        camera.goForward();
    } else if (key == 's') {
        camera.goBack();
    } else if (key == 'm') {
        captureMouse = !captureMouse;
        if (captureMouse) {
            glutWarpPointer(screenWidth / 2, screenHeight / 2);
            glutSetCursor(GLUT_CURSOR_NONE);
        } else {
            glutSetCursor(GLUT_CURSOR_RIGHT_ARROW);
        }
    }
	else if (key == 'a')
		if (msaa)
		{
			glDisable(GL_MULTISAMPLE_ARB);
			msaa = 0;
		}
		else
		{
			glEnable(GL_MULTISAMPLE_ARB);
			msaa = 1;
		}
	
}

// Обработка события нажатия специальных клавиш
void SpecialButtons(int key, int x, int y) {
    if (key == GLUT_KEY_RIGHT) {
        camera.rotateY(0.02);
    } else if (key == GLUT_KEY_LEFT) {
        camera.rotateY(-0.02);
    } else if (key == GLUT_KEY_UP) {
        camera.rotateTop(-0.02);
    } else if (key == GLUT_KEY_DOWN) {
        camera.rotateTop(0.02);
    }
}

void IdleFunc() {
    glutPostRedisplay();
}

// Обработка события движения мыши
void MouseMove(int x, int y) {
    if (captureMouse) {
        int centerX = screenWidth / 2,
            centerY = screenHeight / 2;
        if (x != centerX || y != centerY) {
            camera.rotateY((x - centerX) / 1000.0f);
            camera.rotateTop((y - centerY) / 1000.0f);
            glutWarpPointer(centerX, centerY);
        }
    }
}

// Обработка нажатия кнопки мыши
void MouseClick(int button, int state, int x, int y) {
}

// Событие изменение размера окна
void windowReshapeFunc(GLint newWidth, GLint newHeight) {
    glViewport(0, 0, newWidth, newHeight);
    screenWidth = newWidth;
    screenHeight = newHeight;

    camera.screenRatio = (float)screenWidth / screenHeight;
}

// Инициализация окна
void InitializeGLUT(int argc, char **argv) {

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_MULTISAMPLE);
    glutInitContextVersion(3, 1);
    glutInitWindowPosition(-1, -1);
    glutInitWindowSize(screenWidth, screenHeight);
    glutCreateWindow("Computer Graphics 3");
    glutWarpPointer(400, 300);
    glutSetCursor(GLUT_CURSOR_NONE);

    glutDisplayFunc(RenderLayouts);
    glutKeyboardFunc(KeyboardEvents);
    glutSpecialFunc(SpecialButtons);
    glutIdleFunc(IdleFunc);
    glutPassiveMotionFunc(MouseMove);
    glutMouseFunc(MouseClick);
    glutReshapeFunc(windowReshapeFunc);
}

vector<VM::vec2> GeneratePositions(GLuint INSTANCES) {
	vector<VM::vec2> Positions(INSTANCES);
	for (uint i = 0; i < INSTANCES; ++i)
	{
		Positions[i]
			= VM::vec2((rand() % static_cast<int>(sqrt(RAND_MAX + 1000))) / static_cast<float>(sqrt(RAND_MAX + 1000)), (rand() / static_cast<int>(sqrt(RAND_MAX + 1000))) / static_cast<float>(sqrt(RAND_MAX + 10)))
			+ VM::vec2(0, 1) / 1000.0;
	}
	return Positions;
}
//меш куба
vector<VM::vec4> genmeshCube()
{
	return {
		VM::vec4(-1, 1, -1, 1),
		VM::vec4(-1, -1, -1, 1),
		VM::vec4(1, -1, -1, 1),
		VM::vec4(1, -1, -1, 1),
		VM::vec4(1, 1, -1, 1),
		VM::vec4(-1, 1, -1, 1),
		VM::vec4(-1, -1, 1, 1),
		VM::vec4(-1, -1, -1, 1),
		VM::vec4(-1, 1, -1, 1),
		VM::vec4(-1, 1, -1, 1),
		VM::vec4(-1, 1, 1, 1),
		VM::vec4(-1, -1, 1, 1),
		VM::vec4(1, -1, -1, 1),
		VM::vec4(1, -1, 1, 1),
		VM::vec4(1, 1, 1, 1),
		VM::vec4(1, 1, 1, 1),
		VM::vec4(1, 1, -1, 1),
		VM::vec4(1, -1, -1, 1),
		VM::vec4(-1, -1, 1, 1),
		VM::vec4(-1, 1, 1, 1),
		VM::vec4(1, 1, 1, 1),
		VM::vec4(1, 1, 1, 1),
		VM::vec4(1, -1, 1, 1),
		VM::vec4(-1, -1, 1, 1),
		VM::vec4(-1, 1, -1, 1),
		VM::vec4(1, 1, -1, 1),
		VM::vec4(1, 1, 1, 1),
		VM::vec4(1, 1, 1, 1),
		VM::vec4(-1, 1, 1, 1),
		VM::vec4(-1, 1, -1, 1),
		VM::vec4(-1, -1, -1, 1),
		VM::vec4(-1, -1, 1, 1),
		VM::vec4(1, -1, -1, 1),
		VM::vec4(1, -1, -1, 1),
		VM::vec4(-1, -1, 1, 1),
		VM::vec4(1, -1, 1, 1),
	};
}
//создание статичных обьектов
void CreateObject(string name, const char* obj_name, GLuint &obj_pointcount, GLuint &objUV,GLuint& objShader, string shader_name,
	GLuint& obj_texture, const char* wtexture, GLuint& objVAO, int cube)
{
	
	std::vector<VM::vec3> vertices;
	std::vector<VM::vec2> uvs;
	std::vector<VM::vec3> normals;
	bool obj;
	if (cube == 0)
	{
		obj = loadOBJ(obj_name, vertices, uvs, normals);
		obj_pointcount = vertices.size();
		objUV = uvs.size();
	}
	else {
		vertices.push_back(VM::vec3(-1,0,-1));
		vertices.push_back(VM::vec3(1, 0, 1));
		uvs.push_back(VM::vec2(-1, -1));
		uvs.push_back(VM::vec2(1,  1));
		obj_pointcount = 6;
		objUV = 4;
	}
	
	  vector<VM::vec3> poi(1);
	poi[0] = camera.position;

	objShader = GL::CompileShaderProgram(shader_name);
	glBindTexture(GL_TEXTURE_2D, 0);

	glGenTextures(1, &obj_texture);////////////////////////////////////////////////

	glBindTexture(GL_TEXTURE_2D, obj_texture);

	
		obj_texture = SOIL_load_OGL_texture(wtexture, SOIL_LOAD_RGB, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
		if (0 == obj_texture)
			cout << "object loading error: " << SOIL_last_result();

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	GLuint pBuffer;
	glGenBuffers(1, &pBuffer);
	GLuint pBufferUV;
	glGenBuffers(1, &pBufferUV);                                              CHECK_GL_ERRORS

	glGenVertexArrays(1, &objVAO);                                            CHECK_GL_ERRORS
	glBindVertexArray(objVAO);                                                CHECK_GL_ERRORS
	
	glBindBuffer(GL_ARRAY_BUFFER, pBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(VM::vec3), &vertices[0], GL_STATIC_DRAW);

	GLuint index = glGetAttribLocation(objShader, "point3");                   CHECK_GL_ERRORS
		glEnableVertexAttribArray(index);                                            CHECK_GL_ERRORS
		glVertexAttribPointer(index, 3, GL_FLOAT, GL_FALSE, 0, 0);

		glBindBuffer(GL_ARRAY_BUFFER, pBufferUV);                                 CHECK_GL_ERRORS
		glBufferData(GL_ARRAY_BUFFER, sizeof(VM::vec2) * uvs.size(), &uvs[0], GL_STATIC_DRAW); CHECK_GL_ERRORS

		GLuint indexUV = glGetAttribLocation(objShader, "texCoord");                   CHECK_GL_ERRORS
		glEnableVertexAttribArray(indexUV);                                            CHECK_GL_ERRORS
		glVertexAttribPointer(indexUV, 2, GL_FLOAT, GL_FALSE, 0, 0);					 CHECK_GL_ERRORS

	glBindVertexArray(0);                                                        CHECK_GL_ERRORS
	glBindBuffer(GL_ARRAY_BUFFER, 0);                                            CHECK_GL_ERRORS

}

// кубическое небо
void CreateSkybox(const char* obj_name, GLuint &obj_pointcount, GLuint &objUV, GLuint& objShader, string shader_name,
	GLuint& obj_texture, GLuint& objVAO)
{
	vector<VM::vec4> skyPoints = genmeshCube();
	obj_pointcount = 36;

	vector<VM::vec3> poi(1);
	poi[0] = camera.position;

	objShader = GL::CompileShaderProgram(shader_name);
	
	glGenTextures(1, &obj_texture);
	glBindTexture(GL_TEXTURE_CUBE_MAP, obj_texture);

	obj_texture = SOIL_load_OGL_cubemap("../bin/Texture/right.jpg", "../bin/Texture/left.jpg", "../bin/Texture/top.jpg",
		"../bin/Texture/bottom.jpg", "../bin/Texture/back.jpg", "../bin/Texture/front.jpg", SOIL_LOAD_RGBA, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
	if (0 == obj_texture)
		cout << "object loading error: " << SOIL_last_result();

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		
		GLuint pBuffer;
		glGenBuffers(1, &pBuffer);
		GLuint pBufferUV;
		glGenBuffers(1, &pBufferUV);                                              CHECK_GL_ERRORS

		glGenVertexArrays(1, &objVAO);                                            CHECK_GL_ERRORS
		glBindVertexArray(objVAO);                                                CHECK_GL_ERRORS

		glBindBuffer(GL_ARRAY_BUFFER, pBuffer);
		glBufferData(GL_ARRAY_BUFFER, skyPoints.size() * sizeof(VM::vec4), skyPoints.data(), GL_STATIC_DRAW);

		GLuint index = glGetAttribLocation(objShader, "point4");                   CHECK_GL_ERRORS
		glEnableVertexAttribArray(index);                                            CHECK_GL_ERRORS
		glVertexAttribPointer(index, 4, GL_FLOAT, GL_FALSE, 0, 0);

	//
		//			GLuint camBuffer;
		//		glGenBuffers(1, &camBuffer);                                            CHECK_GL_ERRORS
		//			// Здесь мы привязываем новый буфер, так что дальше вся работа будет с ним до следующего вызова glBindBuffer
		//			glBindBuffer(GL_ARRAY_BUFFER, camBuffer);                               CHECK_GL_ERRORS
		//			glBufferData(GL_ARRAY_BUFFER, sizeof(VM::vec3) * sizeof(poi), poi.data(), GL_STATIC_DRAW); CHECK_GL_ERRORS

		glBindVertexArray(0);                                                        CHECK_GL_ERRORS
		glBindBuffer(GL_ARRAY_BUFFER, 0);                                            CHECK_GL_ERRORS

		//glGenTextures(1, &cam);												CHECK_GL_ERRORS;
		//
		//		glBindTexture(GL_TEXTURE_BUFFER, cam);                                  CHECK_GL_ERRORS;
		//		glTexBuffer(GL_TEXTURE_BUFFER, GL_RG32F, camBuffer);                    CHECK_GL_ERRORS;
		//		glBindTexture(GL_TEXTURE_BUFFER, 0);	                                     CHECK_GL_ERRORS;
}
//меш один и тот же, в шейдерах потом умножается на щепотку рандома
vector<VM::vec4> GenMesh(uint n) {
    return {
        VM::vec4(0, 0, 0, 1),
        VM::vec4(0.5, 0, 0, 1),
        VM::vec4(0, 0.5, 0, 1),
				   
		VM::vec4(0.5, 0, 0, 1),
		VM::vec4(0, 0.5, 0, 1),
		VM::vec4(0.45, 0.5, 0, 1),

		VM::vec4(0, 0.5, 0, 1),
		VM::vec4(0.45, 0.5, 0, 1),
		VM::vec4(0, 0.75, 0, 1),

		VM::vec4(0.45, 0.5, 0, 1),
		VM::vec4(0, 0.75, 0, 1),
		VM::vec4(0.4, 0.75, 0, 1),

		VM::vec4(0, 0.75, 0, 1),
		VM::vec4(0.4, 0.75, 0, 1),
		VM::vec4(0.1, 1 , 0, 1),

		VM::vec4(0, 0, 0, 1),
		VM::vec4(0.5, 0, 0.5, 1),
		VM::vec4(0, 0.5, 0, 1),

		VM::vec4(0.5, 0, 0.5, 1),
		VM::vec4(0, 0.5, 0, 1),
		VM::vec4(0.45, 0.5, 0.5, 1),	

		VM::vec4(0, 0.5, 0, 1),
		VM::vec4(0.45, 0.5, 0.5, 1),
		VM::vec4(0, 0.75, 0, 1),

		VM::vec4(0.45, 0.5, 0.5, 1),
		VM::vec4(0, 0.75, 0.5, 1),
		VM::vec4(0.4, 0.75, 0, 1),

		VM::vec4(0, 0.75, 0.5, 1),
		VM::vec4(0.4, 0.75, 0, 1),
		VM::vec4(0.1, 1 , 0, 1)
	};
}
//создание подвижных обьектов
void CreatePlant()
{
	std::vector<VM::vec3> vertices;
	std::vector<VM::vec2> uvs;
	std::vector<VM::vec3> normals;


	bool obj = loadOBJ("../bin/Objects/34/plant.obj", vertices, uvs, normals);

	plantPointsCount = vertices.size();

	//cloverPointsCount = cloverPoints.size();
	vector<VM::vec2> plantPositions = GeneratePositions(plant_INSTANCES);

	for (uint i = 0; i < plant_INSTANCES; ++i)
	{
		plantVarianceData[i] = VM::vec4(0, 0, 0, 0);
		plantV[i] = VM::vec4(0, 0, 0, 0);
		plantA[i] = VM::vec4(0, 0, 0, 0);
	}

	plantShader = GL::CompileShaderProgram("plant");	CHECK_GL_ERRORS

		GLuint pbuf; CHECK_GL_ERRORS
		glGenBuffers(1, &pbuf);	CHECK_GL_ERRORS
		glBindBuffer(GL_ARRAY_BUFFER, pbuf); CHECK_GL_ERRORS
		glBufferData(GL_ARRAY_BUFFER, sizeof(VM::vec3)*vertices.size(), &vertices[0], GL_STATIC_DRAW); CHECK_GL_ERRORS

		glGenVertexArrays(1, &plantVAO); CHECK_GL_ERRORS
		glBindVertexArray(plantVAO); CHECK_GL_ERRORS


		GLuint pointsLocation = glGetAttribLocation(plantShader, "point"); CHECK_GL_ERRORS
		glEnableVertexAttribArray(pointsLocation); CHECK_GL_ERRORS
		glVertexAttribPointer(pointsLocation, 3, GL_FLOAT, GL_FALSE, 0, 0); CHECK_GL_ERRORS

		// Создаём буфер для позиций 
		GLuint positionBuffer;
	glGenBuffers(1, &positionBuffer);                                            CHECK_GL_ERRORS
		// Здесь мы привязываем новый буфер, так что дальше вся работа будет с ним до следующего вызова glBindBuffer
		glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);                               CHECK_GL_ERRORS
		glBufferData(GL_ARRAY_BUFFER, sizeof(VM::vec2) * plantPositions.size(), plantPositions.data(), GL_STATIC_DRAW); CHECK_GL_ERRORS

		glGenBuffers(1, &plantVariance);                                            CHECK_GL_ERRORS
		glBindBuffer(GL_ARRAY_BUFFER, plantVariance);                               CHECK_GL_ERRORS
		glBufferData(GL_ARRAY_BUFFER, sizeof(VM::vec4) * plant_INSTANCES, plantVarianceData.data(), GL_STATIC_DRAW); CHECK_GL_ERRORS

		int width, height;
	unsigned char* imagem = SOIL_load_image("../bin/Texture/plant.jpg", &width, &height, 0, SOIL_LOAD_RGB);

	glBindTexture(GL_TEXTURE_2D, planttexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, imagem);
	glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	SOIL_free_image_data(imagem);
	glBindTexture(GL_TEXTURE_2D, 0);

	GLuint pointUV;
	glGenBuffers(1, &pointUV);			CHECK_GL_ERRORS
		glBindBuffer(GL_ARRAY_BUFFER, pointUV);		CHECK_GL_ERRORS
		glBufferData(GL_ARRAY_BUFFER, sizeof(VM::vec2)*uvs.size(), &uvs[0], GL_STATIC_DRAW);	CHECK_GL_ERRORS

		GLuint index;
	index = glGetAttribLocation(plantShader, "UV");
	glEnableVertexAttribArray(index);
	glVertexAttribPointer(index, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glBindVertexArray(0);			CHECK_GL_ERRORS
		glBindBuffer(GL_ARRAY_BUFFER, 0);  CHECK_GL_ERRORS

		glGenTextures(1, &plantTBO);												CHECK_GL_ERRORS;

	glBindTexture(GL_TEXTURE_BUFFER, plantTBO);                                  CHECK_GL_ERRORS;
	glTexBuffer(GL_TEXTURE_BUFFER, GL_RG32F, positionBuffer);                    CHECK_GL_ERRORS;
	glBindTexture(GL_TEXTURE_BUFFER, 0);	                                     CHECK_GL_ERRORS;
	// ещё раз ... 
	glGenTextures(1, &plantTBO2);                                                 CHECK_GL_ERRORS;
	glBindTexture(GL_TEXTURE_BUFFER, plantTBO2);                                  CHECK_GL_ERRORS;
	glTexBuffer(GL_TEXTURE_BUFFER, GL_RGBA32F, plantVariance);                    CHECK_GL_ERRORS;
	glBindTexture(GL_TEXTURE_BUFFER, 0);	                                      CHECK_GL_ERRORS;
}

void CreateClover()
{
	vector<VM::vec4> cloverPoints = {
		VM::vec4(0, 0, 0, 1),
		VM::vec4(0.006, 0, 0, 1),
		VM::vec4(0, 0.1, 0, 1),//top	
		VM::vec4(0, 0.1, 0, 1),
		VM::vec4(0.006,  0, 0, 1),
		VM::vec4(0.006, 0.1, 0, 1),
		VM::vec4(0, 0,   0,    1),
		VM::vec4(0, 0,   0.006,1),
		VM::vec4(0, 0.1, 0,     1),
		VM::vec4(0, 0,   0.006,1),
		VM::vec4(0, 0.1, 0,     1),
		VM::vec4(0, 0.1,   0.006,1),
		VM::vec4(0, 0.1,   0, 1),
		VM::vec4(0.1, 0.131, 0.05,     1),
		VM::vec4(0.05, 0.132,   0.1, 1),
		VM::vec4(0, 0.1,   0,1),
		VM::vec4(-0.1, 0.132, -0.05,     1),
		VM::vec4(-0.05, 0.133,  -0.1,1),
		VM::vec4(0, 0.1,   0,1),
		VM::vec4(0.1, 0.13, -0.05,     1),
		VM::vec4(0.05, 0.133,  -0.1,1),
		VM::vec4(0, 0.1,   0,1),
		VM::vec4(-0.1, 0.13, 0.05,     1),
		VM::vec4(-0.05, 0.135,  0.1,1),
	};
	vector<VM::vec2> cloverUV = {
		VM::vec2(0, 0),
		VM::vec2(0.006, 0),
		VM::vec2(0, 0.1),
		VM::vec2(0, 0.1  ),
		VM::vec2(0.006,  0  ),
		VM::vec2(0.006, 0.1),
		VM::vec2( 0,   0),
		VM::vec2( 0,   0.006),
		VM::vec2( 0.1, 0),
		VM::vec2(0,   0.006),
		VM::vec2(0.1, 0  ),
		VM::vec2(0.1,   0.006),			   
		VM::vec2( 0.1, 0),
		VM::vec2( 0.05, 0.08 ),
		VM::vec2( 0.05, 0.0 ),			   
		VM::vec2(0, 0.05),
		VM::vec2(0.08, 0.05),
		VM::vec2(0.04, 0.02)
	};
	
	//std::vector<VM::vec3> vertices;
	//std::vector<VM::vec2> uvs;
	//std::vector<VM::vec3> normals;
	//
	//
	//bool obj = loadOBJ("../bin/Objects/34/plant.obj", vertices, uvs, normals);
	//
	//cloverPointsCount = vertices.size();
	
	cloverPointsCount = cloverPoints.size();
	vector<VM::vec2> cloverPositions = GeneratePositions(Clover_INSTANCES);
	
	for (uint i = 0; i < Clover_INSTANCES; ++i)
	{
		cloverVarianceData[i] = VM::vec4(0, 0, 0, 0);
		cloverV[i] = VM::vec4(0, 0, 0, 0);
		cloverA[i] = VM::vec4(0, 0, 0, 0);
	}
	
	cloverShader = GL::CompileShaderProgram("clover");	CHECK_GL_ERRORS

	GLuint pbuf; CHECK_GL_ERRORS
	glGenBuffers(1, &pbuf);	CHECK_GL_ERRORS
	glBindBuffer(GL_ARRAY_BUFFER, pbuf); CHECK_GL_ERRORS
	glBufferData( GL_ARRAY_BUFFER, sizeof(VM::vec4)*cloverPoints.size(), cloverPoints.data(), GL_STATIC_DRAW); CHECK_GL_ERRORS

//		GLuint pbuf; CHECK_GL_ERRORS
//		glGenBuffers(1, &pbuf);	CHECK_GL_ERRORS
//		glBindBuffer(GL_ARRAY_BUFFER, pbuf); CHECK_GL_ERRORS
//		glBufferData(GL_ARRAY_BUFFER, sizeof(VM::vec3)*vertices.size(), &vertices[0], GL_STATIC_DRAW); CHECK_GL_ERRORS
//
		glGenVertexArrays(1, &cloverVAO); CHECK_GL_ERRORS
		glBindVertexArray(cloverVAO); CHECK_GL_ERRORS

		GLuint pointsLocation = glGetAttribLocation(cloverShader, "point"); CHECK_GL_ERRORS
		glEnableVertexAttribArray(pointsLocation); CHECK_GL_ERRORS
		glVertexAttribPointer(pointsLocation, 4, GL_FLOAT, GL_FALSE, 0, 0); CHECK_GL_ERRORS

		//GLuint pointsLocation = glGetAttribLocation(cloverShader, "point"); CHECK_GL_ERRORS
		//glEnableVertexAttribArray(pointsLocation); CHECK_GL_ERRORS
		//glVertexAttribPointer(pointsLocation, 3, GL_FLOAT, GL_FALSE, 0, 0); CHECK_GL_ERRORS

		// Создаём буфер для позиций 
		GLuint positionBuffer;
		glGenBuffers(1, &positionBuffer);                                            CHECK_GL_ERRORS
		// Здесь мы привязываем новый буфер, так что дальше вся работа будет с ним до следующего вызова glBindBuffer
		glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);                               CHECK_GL_ERRORS
		glBufferData(GL_ARRAY_BUFFER, sizeof(VM::vec2) * cloverPositions.size(), cloverPositions.data(), GL_STATIC_DRAW); CHECK_GL_ERRORS

		glGenBuffers(1, &cloverVariance);                                            CHECK_GL_ERRORS
		glBindBuffer(GL_ARRAY_BUFFER, cloverVariance);                               CHECK_GL_ERRORS
		glBufferData(GL_ARRAY_BUFFER, sizeof(VM::vec4) * Clover_INSTANCES, cloverVarianceData.data(), GL_STATIC_DRAW); CHECK_GL_ERRORS


		clovertexture = SOIL_load_OGL_texture("../bin/Texture/ground.jpg", SOIL_LOAD_RGB, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
	if (0 == grasstexture)
		cout << "SOIL loading error: " << SOIL_last_result();
	glBindTexture(GL_TEXTURE_2D, clovertexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

//	GLuint pointUV;
//	glGenBuffers(1, &pointUV);			CHECK_GL_ERRORS
//		glBindBuffer(GL_ARRAY_BUFFER, pointUV);		CHECK_GL_ERRORS
//		glBufferData(GL_ARRAY_BUFFER, sizeof(VM::vec2)*uvs.size(), &uvs[0], GL_STATIC_DRAW);	CHECK_GL_ERRORS
	
		GLuint pointUV;
	glGenBuffers(1, &pointUV);			CHECK_GL_ERRORS
		glBindBuffer(GL_ARRAY_BUFFER, pointUV);		CHECK_GL_ERRORS
		glBufferData(GL_ARRAY_BUFFER, sizeof(VM::vec2)*cloverUV.size(), cloverUV.data(), GL_STATIC_DRAW);	CHECK_GL_ERRORS

	GLuint index;
	index = glGetAttribLocation(cloverShader, "UV");
	glEnableVertexAttribArray(index);
	glVertexAttribPointer(index, 2, GL_FLOAT, GL_FALSE, 0, 0);

		glBindVertexArray(0);			CHECK_GL_ERRORS
		glBindBuffer(GL_ARRAY_BUFFER, 0);  CHECK_GL_ERRORS

		glGenTextures(1, &cloverTBO);												CHECK_GL_ERRORS;

		glBindTexture(GL_TEXTURE_BUFFER, cloverTBO);                                  CHECK_GL_ERRORS;
		glTexBuffer(GL_TEXTURE_BUFFER, GL_RG32F, positionBuffer);                    CHECK_GL_ERRORS;
		glBindTexture(GL_TEXTURE_BUFFER, 0);	                                     CHECK_GL_ERRORS;
		// ещё раз ... 
		glGenTextures(1, &cloverTBO2);                                                 CHECK_GL_ERRORS;
		glBindTexture(GL_TEXTURE_BUFFER, cloverTBO2);                                  CHECK_GL_ERRORS;
		glTexBuffer(GL_TEXTURE_BUFFER, GL_RGBA32F, cloverVariance);                    CHECK_GL_ERRORS;
		glBindTexture(GL_TEXTURE_BUFFER, 0);	                                      CHECK_GL_ERRORS;
}

// Создание травы
void CreateGrass() {
    uint LOD = 1;
    // Создаём меш
    vector<VM::vec4> grassPoints = GenMesh(LOD);
	
    // Сохраняем количество вершин в меше травы
    grassPointsCount = grassPoints.size();
    // Создаём позиции для травинок
    vector<VM::vec2> grassPositions = GeneratePositions(GRASS_INSTANCES);
	//vector<VM::vec2> grassRotate = GenerateGrassRotate();

    // Инициализация смещений для травинок
    for (uint i = 0; i < GRASS_INSTANCES; ++i) {
        grassVarianceData[i] = VM::vec4(0, 0, 0, 0);
		grassV[i] = VM::vec4(0, 0, 0, 0);
		grassA[i] = VM::vec4(0, 0, 0, 0);
	}

    grassShader = GL::CompileShaderProgram("grass");

    // Здесь создаём буфер
    GLuint pointsBuffer;
    // Это генерация одного буфера (в pointsBuffer хранится идентификатор буфера)
    glGenBuffers(1, &pointsBuffer);                                              CHECK_GL_ERRORS
    // Привязываем сгенерированный буфер
    glBindBuffer(GL_ARRAY_BUFFER, pointsBuffer);                                 CHECK_GL_ERRORS
    // Заполняем буфер данными из вектора
    glBufferData(GL_ARRAY_BUFFER, sizeof(VM::vec4) * grassPoints.size(), grassPoints.data(), GL_STATIC_DRAW); CHECK_GL_ERRORS

    glGenVertexArrays(1, &grassVAO);                                             CHECK_GL_ERRORS
    // Привязка VAO
    glBindVertexArray(grassVAO);                                                 CHECK_GL_ERRORS

    GLuint pointsLocation = glGetAttribLocation(grassShader, "point");           CHECK_GL_ERRORS
    // Подключаем массив атрибутов к данной локации
    glEnableVertexAttribArray(pointsLocation);                                   CHECK_GL_ERRORS
    // Устанавливаем параметры для получения данных из массива (по 4 значение типа float на одну вершину)
    glVertexAttribPointer(pointsLocation, 4, GL_FLOAT, GL_FALSE, 0, 0);          CHECK_GL_ERRORS

    // Создаём буфер для позиций травинок
    GLuint positionBuffer;
    glGenBuffers(1, &positionBuffer);                                            CHECK_GL_ERRORS
    // Здесь мы привязываем новый буфер, так что дальше вся работа будет с ним до следующего вызова glBindBuffer
    glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);                               CHECK_GL_ERRORS
    glBufferData(GL_ARRAY_BUFFER, sizeof(VM::vec2) * grassPositions.size(), grassPositions.data(), GL_STATIC_DRAW); CHECK_GL_ERRORS

    glGenBuffers(1, &grassVariance);                                            CHECK_GL_ERRORS
    glBindBuffer(GL_ARRAY_BUFFER, grassVariance);                               CHECK_GL_ERRORS
    glBufferData(GL_ARRAY_BUFFER, sizeof(VM::vec4) * GRASS_INSTANCES, grassVarianceData.data(), GL_STATIC_DRAW); CHECK_GL_ERRORS

		grasstexture[0] = SOIL_load_OGL_texture("../bin/Texture/plant.jpg", SOIL_LOAD_RGB, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
	if (0 == grasstexture[0])
		cout << "SOIL loading error: " << SOIL_last_result();
	glBindTexture(GL_TEXTURE_2D, grasstexture[0]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glBindTexture(GL_TEXTURE_2D, 0);

	grasstexture[1] = SOIL_load_OGL_texture("../bin/Texture/grass.jpg", SOIL_LOAD_RGB, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
	if (0 == grasstexture[1])
		cout << "SOIL loading error: " << SOIL_last_result();
	glBindTexture(GL_TEXTURE_2D, grasstexture[1]);
	glBindTexture(GL_TEXTURE_2D, 0);

// Отвязываем VAO
    glBindVertexArray(0);                                                        CHECK_GL_ERRORS
    // Отвязываем буфер
    glBindBuffer(GL_ARRAY_BUFFER, 0);                                            CHECK_GL_ERRORS

    // создание иинициализация фиктивной текстуры для чтения из TBO в шейдере
    glGenTextures(1, &grassTBO);												CHECK_GL_ERRORS;
    
	glBindTexture(GL_TEXTURE_BUFFER, grassTBO);                                  CHECK_GL_ERRORS;
    glTexBuffer(GL_TEXTURE_BUFFER, GL_RG32F, positionBuffer);                    CHECK_GL_ERRORS;
    glBindTexture(GL_TEXTURE_BUFFER, 0);	                                     CHECK_GL_ERRORS;
    // ещё раз ... 
    glGenTextures(1, &grassTBO2);                                                 CHECK_GL_ERRORS;
    glBindTexture(GL_TEXTURE_BUFFER, grassTBO2);                                  CHECK_GL_ERRORS;
    glTexBuffer(GL_TEXTURE_BUFFER, GL_RGBA32F, grassVariance);                    CHECK_GL_ERRORS;
    glBindTexture(GL_TEXTURE_BUFFER, 0);	       CHECK_GL_ERRORS;
}

// Создаём камеру (Если шаблонная камера вам не нравится, то можете переделать, но я бы не стал)
void CreateCamera() {
    camera.angle = 45.0f / 180.0f * M_PI;
    camera.direction = VM::vec3(0, 0.3, -1);
    camera.position = VM::vec3(0.5, 0.2, 0.5);
    camera.screenRatio = (float)screenWidth / screenHeight;
    camera.up = VM::vec3(0, 1, 0);
    camera.zfar = 50.0f;
    camera.znear = 0.05f;
}
//гора или камень в виде тетраида
void CreateMount() {
	// Земля состоит из двух треугольников
	vector<VM::vec4> meshPointsm = {

		VM::vec4(0, 0, 0, 1),
		VM::vec4(0, 0, 0.5, 1),
		VM::vec4(0.15, 1, 0.25, 1), //top
		
		VM::vec4(0, 0, 0, 1),
		VM::vec4(0.5, 0,0,1), 
		VM::vec4(0.15, 1, 0.25, 1),//top

		VM::vec4(0, 0, 0.5, 1),
		VM::vec4(0.5, 0,0,1),
		VM::vec4(0.15, 1, 0.25, 1)//top
	};
	vector<VM::vec2> meshPointsUVm = {
		VM::vec2(1, 1),
		VM::vec2(1, 0),
		VM::vec2(0, 1),

		VM::vec2(1, 1),
		VM::vec2(1, 0),
		VM::vec2(0, 1),

		VM::vec2(1, 1),
		VM::vec2(1, 0),
		VM::vec2(0, 1),
	};
	// Подробнее о том, как это работает читайте в функции CreateGrass

	mountShader = GL::CompileShaderProgram("mount");


	mounttexture= SOIL_load_OGL_texture("../bin/Texture/stone.jpg", SOIL_LOAD_RGB, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
	if (0 == mounttexture)
	cout<<"MOUNT loading error: "<< SOIL_last_result();
	glBindTexture(GL_TEXTURE_2D, mounttexture);
	
		glGenVertexArrays(1, &mountVAO);                                            CHECK_GL_ERRORS
		glBindVertexArray(mountVAO);                                                CHECK_GL_ERRORS
	
		GLuint pBuffer;
		glGenBuffers(1, &pBuffer);                                              CHECK_GL_ERRORS
		glBindBuffer(GL_ARRAY_BUFFER, pBuffer);                                 CHECK_GL_ERRORS
		glBufferData(GL_ARRAY_BUFFER, sizeof(VM::vec4) * meshPointsm.size(), meshPointsm.data(), GL_STATIC_DRAW); CHECK_GL_ERRORS


		GLuint index = glGetAttribLocation(mountShader, "point");                   CHECK_GL_ERRORS
		glEnableVertexAttribArray(index);                                            CHECK_GL_ERRORS
		glVertexAttribPointer(index, 4, GL_FLOAT, GL_FALSE, 0, 0);

		GLuint pBufferUV;
		glGenBuffers(1, &pBufferUV);                                              CHECK_GL_ERRORS
		glBindBuffer(GL_ARRAY_BUFFER, pBufferUV);                                 CHECK_GL_ERRORS
		glBufferData(GL_ARRAY_BUFFER, sizeof(VM::vec2) * meshPointsUVm.size(), meshPointsUVm.data(), GL_STATIC_DRAW); CHECK_GL_ERRORS

		GLuint indexUV = glGetAttribLocation(mountShader, "texCoord");                   CHECK_GL_ERRORS
		glEnableVertexAttribArray(indexUV);                                            CHECK_GL_ERRORS
		glVertexAttribPointer(indexUV, 2, GL_FLOAT, GL_FALSE, 0, 0);					 CHECK_GL_ERRORS

	glBindVertexArray(0);                                                        CHECK_GL_ERRORS
	glBindBuffer(GL_ARRAY_BUFFER, 0);                                            CHECK_GL_ERRORS

}

// Создаём замлю
void CreateGround() {

// Земля состоит из двух треугольников
    vector<VM::vec4> meshPoints = {
        VM::vec4(0, 0, 0, 1),
        VM::vec4(1.25, 0, 0, 1),
        VM::vec4(1.25, 0, 1, 1),
        VM::vec4(0, 0, 0, 1),
        VM::vec4(1.25, 0, 1, 1),
        VM::vec4(0, 0, 1, 1),
    };
	vector<VM::vec2> meshPointsUV = {
		VM::vec2(0, 0),
		VM::vec2(1.15, 0),
		VM::vec2(1.15, 1),
		VM::vec2(0, 0),
		VM::vec2(1.15, 1),
		VM::vec2(0, 1),
	};
    // Подробнее о том, как это работает читайте в функции CreateGrass

    groundShader = GL::CompileShaderProgram("ground");

  	groundtexture= SOIL_load_OGL_texture("../bin/Texture/ground3.jpg", SOIL_LOAD_RGB, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
  	if (0 == groundtexture)
  	cout<<"SOIL loading error: "<< SOIL_last_result();
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glBindTexture(GL_TEXTURE_2D, groundtexture);

	glGenVertexArrays(1, &groundVAO);                                            CHECK_GL_ERRORS
	glBindVertexArray(groundVAO);                                                CHECK_GL_ERRORS
	
    GLuint pointsBuffer;
    glGenBuffers(1, &pointsBuffer);                                              CHECK_GL_ERRORS
    glBindBuffer(GL_ARRAY_BUFFER, pointsBuffer);                                 CHECK_GL_ERRORS
    glBufferData(GL_ARRAY_BUFFER, sizeof(VM::vec4) * meshPoints.size(), meshPoints.data(), GL_STATIC_DRAW); CHECK_GL_ERRORS

	GLuint index = glGetAttribLocation(groundShader, "point");                   CHECK_GL_ERRORS
	glEnableVertexAttribArray(index);                                            CHECK_GL_ERRORS
	glVertexAttribPointer(index, 4, GL_FLOAT, GL_FALSE, 0, 0);

	GLuint pointsBufferUV;
	glGenBuffers(1, &pointsBufferUV);                                              CHECK_GL_ERRORS
	glBindBuffer(GL_ARRAY_BUFFER, pointsBufferUV);                                 CHECK_GL_ERRORS
	glBufferData(GL_ARRAY_BUFFER, sizeof(VM::vec2) * meshPointsUV.size(), meshPointsUV.data(), GL_STATIC_DRAW); CHECK_GL_ERRORS

	GLuint indexUV = glGetAttribLocation(groundShader, "texCoord");                   CHECK_GL_ERRORS
	glEnableVertexAttribArray(indexUV);                                            CHECK_GL_ERRORS
	glVertexAttribPointer(indexUV, 2, GL_FLOAT, GL_FALSE, 0, 0);
             // CHECK_GL_ERRORS

    glBindVertexArray(0);                                                        CHECK_GL_ERRORS
    glBindBuffer(GL_ARRAY_BUFFER, 0);                                            CHECK_GL_ERRORS
		
}

int main(int argc, char **argv)
{
    try {
        cout << "Start" << endl;
        InitializeGLUT(argc, argv);
        cout << "GLUT inited" << endl;
        glewInit();
        cout << "glew inited" << endl;
        CreateCamera();
        cout << "Camera created" << endl;
      
		CreateGrass();
        cout << "Grass created" << endl;
		CreateMount();
		cout << "Mountant created" << endl;
		
		CreateObject("water","../bin/Objects/34/cube.obj", waterPointsCount, waterUV, waterShader, "water", watertexture, "../bin/Texture/water3.jpg", waterVAO,0);
		cout << "water" << endl;

		cout << "Horse is creating ------->  "  ;
		CreateObject("horse","../bin/Objects/34/horse.obj", horsePointsCount, horseUV, horseShader, "horse", horsetexture, "../bin/Texture/horse.jpg", horseVAO,0);
		cout << "Horse was created!!" << endl;

		cout << "House is creating ------->  ";
		CreateObject("house","../bin/Objects/34/house.obj", housePointsCount, houseUV, houseShader, "house", housetexture, "../bin/Texture/marble.jpg", houseVAO,0);
		cout << "House was created!!" << endl;

		cout << "Butterfly is creating ------->  ";
		CreateObject("butterfly", "../bin/Objects/34/butterfly.obj", butPointsCount, butUV, butShader, "but", buttexture, "../bin/Texture/but2.jpg", butVAO, 0);
		cout << "Butterfly was created!!" << endl;

		CreateGround();
        cout << "Ground created" << endl;

		cout << "Clover is creating ------->  ";
		CreateClover();
		cout << "Clover was created" << endl;

		cout << "Firts tree is creating ------->  ";
		CreateObject("tree","../bin/Objects/34/tree.obj", treePointsCount, treeUV, treeShader, "tree", treetexture, "../bin/Texture/plant.jpg", treeVAO,0);
		
	//	cout << "Second tree is creating ------->  ";
	//	CreateObject("tree", "../bin/Objects/34/tree.obj", streePointsCount, streeUV, streeShader, "tree1", streetexture, "../bin/Texture/green.jpg", streeVAO, 0);
	//	cout << "Trees were created!!!";
		

		cout << "Plant is creating ------->  ";
		CreatePlant();
		cout << "Plant was created" << endl;

		CreateSkybox("../bin/Objects/34/cube.obj", skyPointsCount, skyUV, skyShader, "sky", skytexture, skyVAO);
		cout << "Sky" << endl;

		glutMainLoop();
    } catch (string s) {
        cout << s << endl;
    }
}
