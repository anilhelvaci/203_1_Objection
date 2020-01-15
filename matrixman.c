#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

typedef struct vector {
    char* name;
    int* array;
    int arraySize;
} VECTOR;

typedef struct matrix {
    char* name;
    int** multiArray;
    int height, width;
} MATRIX;

VECTOR* initializeVector(char* name, int size, int* array) {
    VECTOR* newVector = malloc(sizeof(VECTOR));
    newVector->name = malloc(sizeof(char) * (strlen(name) + 1));
    strcpy(newVector->name, name);
    newVector->array = array;
    newVector->arraySize = size;
    return newVector;
}

void freeVector(VECTOR* vector) {
    free(vector->name);
    free(vector->array);
    free(vector);
}

void displayVector(int* array, int size, FILE *file) {
    int i = 0;
    for (i = 0; i < size - 1; ++i) {
        fprintf(file, "%d ", array[i]);
    }
    fprintf(file, "%d\n", array[i]);
}

MATRIX* initializeMatrix(char* name, int height, int width, int** multiArray) {
    MATRIX* newMatrix = malloc(sizeof(MATRIX));
    newMatrix->name = malloc(sizeof(char) * (strlen(name) + 1));
    strcpy(newMatrix->name, name);
    newMatrix-> multiArray = multiArray;
    newMatrix->height = height;
    newMatrix->width = width;
    return newMatrix;
}

void freeMatrix(MATRIX* matrix) {
    int i = 0;
    for (i = 0; i < matrix->height; i ++) {
        free(matrix->multiArray[i]);
    }
    free(matrix->multiArray);
    free(matrix->name);
    free(matrix);
}

void freeAllMatrices(MATRIX** matrices, int totalCount) {
    int i;
    for (i = 0; i < totalCount; ++i) {
        freeMatrix(matrices[i]);
    }
}

void freeAllVectors(VECTOR** vectors, int totalcount) {
    int i;
    for (i = 0; i < totalcount; ++i) {
        freeVector(vectors[i]);
    }
}

void displayRow(int* array, int width, FILE *file) {
    int i = 0;
    for (i = 0; i < width - 1; ++i) {
        fprintf(file, "%d " , array[i]);
    }
    fprintf(file, "%d\n", array[i]);
}

void displayMatrix(MATRIX* matrix, FILE *file) {
    int i= 0;
    for (i = 0; i < matrix->height; ++i) {
        displayRow(matrix->multiArray[i], matrix->width, file);
    }
}

int getVectorSizeReadFromFile(const char* string) {
    int i = 0;
    int count = 0;
    while(string[i] != '\0') {
        if (string[i] != ' ') {
            count++;
        }
        i++;
    }
    return count;
}

int* deepCopyArray(int* array, int size) {
    int* copy = malloc(sizeof(int) * size);
    memcpy(copy, array, sizeof(int) * size);
    return copy;
}


int* stringToIntVector(char* stringVector, int *width) {
    int* intVector = malloc(sizeof(int));
    int i = 0;
    int count = 0;

    char* value = strtok(stringVector, " ");

    intVector[i] = atoi(value);

    while (value) {
        intVector = realloc(intVector, sizeof(int) * (i + 1));
        intVector[i] = atoi(value);
        i++;
        value = strtok(NULL, " ");
    }
    *width = i;
    return intVector;
}

int** createZeroMatrix(int height, int width) {
    int** zeroMatrix = malloc(sizeof(int*) * height);
    int i = 0;
    for (i = 0; i < height; i ++) {
        zeroMatrix[i] = calloc(width, sizeof(int));
    }
    return zeroMatrix;
}

MATRIX* createMatrixFromString(char** vectorsAsString, int height, char* name) {
    int** readMatrix = malloc(sizeof(int*) * height);
    int width = 0;
    int i = 0;

    for (i = 0; i < height; ++i) {
        readMatrix[i] = stringToIntVector(vectorsAsString[i], &width);
    }

    return initializeMatrix(name, height, width, readMatrix);
}

char* readAllCharacters(char* filePath) {
    FILE *file;
    file = fopen(filePath, "r"); if (file == NULL) return NULL;
    char* characters = malloc(sizeof(char));
    int charCount = 0;
    int c = fgetc(file);

    while (c != EOF) {
        characters[charCount] = c;
        charCount++;
        characters = realloc(characters, sizeof(char) * (charCount + 1));
        c = fgetc(file);
    }

    characters[charCount] = '\0';
    fclose(file);
    return characters;
}

int getHeightOfMatrixFromFile(char* path) {
    char* splitString = readAllCharacters(path);
    char* vector = strtok(splitString, "\r\n");
    int i = 0;
    while (vector) {
        vector = strtok(NULL, "\r\n");
        i++;
    }
    free(splitString);
    return i;
}

char** charsToLines(char* characters) {
    char** commandLines = malloc(sizeof(char*) * 2);
    char* line = strtok(characters, "\r\n");
    int i = 1;
    commandLines[0] = line;

    while (line) {
        line = strtok(NULL, "\r\n");
        commandLines[i] = line;
        i++;
        commandLines = realloc(commandLines, sizeof(char*) * (i + 1));
    }
    return commandLines;
}

VECTOR** addVector(VECTOR* vector, VECTOR** vectorArray, int* vectorCount) {
    vectorArray = realloc(vectorArray, sizeof(VECTOR*) * (*vectorCount + 1));
    vectorArray[*vectorCount] = vector;
    *vectorCount = *vectorCount + 1;
    return vectorArray;
}

VECTOR** vecZeros(char* name, char* length, VECTOR** vectorArray, int* vectorCount, FILE *file) {
    VECTOR* newZeroVector = initializeVector(name, atoi(length), calloc(atoi(length), sizeof(int)));
    fprintf(file, "created vector %s %d\n", newZeroVector->name, newZeroVector->arraySize);
    displayVector(newZeroVector->array, newZeroVector->arraySize, file);
    return addVector(newZeroVector, vectorArray, vectorCount);
}

MATRIX** addMatrix(MATRIX* matrix, MATRIX** matrixArray, int* matrixCount) {
    matrixArray = realloc(matrixArray, sizeof(VECTOR*) * (*matrixCount + 1));
    matrixArray[*matrixCount] = matrix;
    *matrixCount = *matrixCount + 1;
    return matrixArray;
}

MATRIX** matZeros(char* name, char* height, char* width, MATRIX** matrixArray, int* matrixCount, FILE *file) {
    MATRIX* newZeroMatrix = initializeMatrix(name, atoi(height), atoi(width), createZeroMatrix(atoi(height), atoi(width)));
    fprintf(file,"created matrix %s %d %d\n", newZeroMatrix->name, newZeroMatrix->height, newZeroMatrix->width);
    displayMatrix(newZeroMatrix, file);
    return addMatrix(newZeroMatrix, matrixArray, matrixCount);
}

VECTOR* buildVectorFromString(char* vecString, char* vecName) {
    int i = 0;
    int count = 0;
    int size = 0;
    int* array = malloc(sizeof(int) * size);
    char* number = strtok_r(vecString, " ", &vecString);
    while (number) {
        if (vecString[i] != ' '){
            array = realloc(array, sizeof(int) * (size + 1)); array[size] = atoi(number);
            count++;
        }
        size++; number = strtok_r(vecString, " ", &vecString);
    }
    return initializeVector(vecName, size, array);
}

VECTOR** vecRead(char* path, char* vecFileName, VECTOR** vectorArray, int* vectorCount, FILE *file) {
    char arrayPath[strlen(path)];
    strcpy(arrayPath, path);
    char* totalPath = strcat(arrayPath, vecFileName);
    char* vecString = readAllCharacters(totalPath); if (!vecString) {fprintf(file, "error\n"); return vectorArray;}
    char fileName[strlen(vecFileName)];
    strcpy(fileName, vecFileName);
    VECTOR* newVectorFromFile = buildVectorFromString(vecString, strtok(vecFileName, "."));
    fprintf(file, "read vector %s %d\n", fileName, newVectorFromFile->arraySize);
    displayVector(newVectorFromFile->array, newVectorFromFile->arraySize, file);
    free(vecString);
    return addVector(newVectorFromFile, vectorArray, vectorCount);
}

MATRIX** matRead(char *path, char* name, MATRIX** matrixArray, int* count, FILE *file) {
    char arrayPath[strlen(path)];
    strcpy(arrayPath, path);
    char* totalPath = strcat(arrayPath, name);
    char* matrixString = readAllCharacters(totalPath);

    char fileName[strlen(name)];
    strcpy(fileName, name);

    int size = getHeightOfMatrixFromFile(totalPath);

    char** vectorsAsStrings = malloc(sizeof(char*) * size);
    char* vector = strtok(matrixString, "\r\n");

    int i = 0;

    while (vector) {
        vectorsAsStrings[i] = vector;
        vector = strtok(NULL, "\r\n");
        i++;
    }

    MATRIX* matrixFromFile = createMatrixFromString(vectorsAsStrings, size, strtok(name, "."));
    fprintf(file, "read matrix %s %d %d\n", fileName, matrixFromFile->height, matrixFromFile->width);
    displayMatrix(matrixFromFile, file);
    free(vectorsAsStrings);
    free(matrixString);
    return addMatrix(matrixFromFile, matrixArray, count);
}

MATRIX** vecStack(VECTOR* vector1, VECTOR* vector2, char* direction, char* name, MATRIX** matrices, int* count, FILE *file) {

    MATRIX* newMatrix = NULL;
    int height = 0;
    int width = 0;

    if (!vector1 || !vector2 || vector1->arraySize != vector2->arraySize) {
        fprintf(file, "error\n");
        return matrices;
    }

    if (strcmp(direction, "column") == 0) {
        height = vector1->arraySize;
        width = 2;
        int** multiArray = malloc(sizeof(int*) * height);

        int i;
        for (i = 0; i < height; i++) {
            int* concatItems = malloc(sizeof(int) * width);
            concatItems[0] = vector1->array[i];
            concatItems[1] = vector2->array[i];
            multiArray[i] = concatItems;
        }
        newMatrix = initializeMatrix(name, height, width, multiArray);
    } else if (strcmp(direction, "row") == 0) {
        height = 2;
        width = vector1->arraySize;

        int** multiArray = malloc(sizeof(int*) * height);
        multiArray[0] = deepCopyArray(vector1->array, vector1->arraySize);
        multiArray[1] = deepCopyArray(vector2->array, vector2->arraySize);
        newMatrix = initializeMatrix(name, height, width, multiArray);
    }

    if (newMatrix != NULL) {
        fprintf(file, "vectors concatenated %s %d %d\n",newMatrix->name, newMatrix->height, newMatrix->width);
    }
    displayMatrix(newMatrix, file);
    return addMatrix(newMatrix, matrices, count);
}

void matStack(MATRIX* matrix1, MATRIX* matrix2, char* where, FILE *file) {
    if (strcmp(where, "r") == 0) {
        if (matrix1->height != matrix2->height) {
            fprintf(file, "error\n");
            return;
        }

        int i;
        for (i = 0; i < matrix1->height; i++) {
            matrix1->multiArray[i] = realloc(matrix1->multiArray[i], sizeof(int) * (matrix1->width + matrix2->width));

            int j;
            for (j = 0; j < matrix2->width; ++j) {
                matrix1->multiArray[i][j + matrix1->width] = matrix2->multiArray[i][j];
            }
        }

        matrix1->width = matrix1->width + matrix2->width;
    } else if (strcmp(where, "d") == 0) {
        if (matrix1->width != matrix2->width) {
            fprintf(file, "Error! Widths are not equal.");
            return;
        }

        int oldHeight = matrix1->height;
        matrix1->height = matrix1->height + matrix2->height;
        matrix1->multiArray = realloc(matrix1->multiArray, sizeof(int*) * matrix1->height);

        int i;
        for (i = 0; i < matrix2->height; ++i) {
            matrix1->multiArray[oldHeight + i] = deepCopyArray(matrix2->multiArray[i], matrix2->width);
        }
    }
    fprintf(file, "matrices concatenated %s %d %d\n", matrix1->name, matrix1->height, matrix1->width);
    displayMatrix(matrix1, file);
}

void mvStack(MATRIX* matrix, VECTOR* vector, char* where, FILE *file) {
    if (strcmp(where, "r") == 0) {
        if (matrix->height != vector->arraySize) {
            fprintf(file, "Error! Vector Matrix sizes do not match!\n");
            return;
        }

        int i;
        for (i = 0; i < matrix->height; ++i) {
            matrix->multiArray[i] = realloc(matrix->multiArray[i], sizeof(int) * (matrix->width + 1));
            matrix->multiArray[i][matrix->width] = vector->array[i];
        }
        matrix->width = matrix->width + 1;
        fprintf(file, "matrix and vector concatenated %s %d %d\n", matrix->name, matrix->height, matrix->width);
        displayMatrix(matrix, file);
    } else if (strcmp(where, "d") == 0) {
        if (matrix->width != vector->arraySize) {
            fprintf(file, "Error! Vector Matrix sizes do not match!\n");
            return;
        }

        matrix->multiArray = realloc(matrix->multiArray, sizeof(int*) * (matrix->height + 1));
        matrix->multiArray[matrix->height] = deepCopyArray(vector->array, vector->arraySize);
        matrix->height++;
        fprintf(file, "matrix and vector concatenated %s %d %d\n", matrix->name, matrix->height, matrix->width);
        displayMatrix(matrix, file);
    }
}

int findMax(int* array, int size) {
    int max = array[0];
    int i;
    for (i = 0; i < size; ++i) {
        if (array[i] > max) {
            max = array[i];
        }
    }
    return max;
}

int findMin(int* array, int size) {
    int min = array[0];
    int i;
    for (i = 0; i < size; ++i) {
        if (array[i] < min) {
            min = array[i];
        }
    }
    return min;
}


int* findColumnMaxes(MATRIX* matrix) {
    int* result = malloc(sizeof(int) * matrix->width);
    int* column = malloc(sizeof(int) * matrix->height);
    int i;
    for (i = 0; i < matrix->width; ++i) {
        int j;
        for (j = 0; j < matrix->height; ++j) {
            column[j] = matrix->multiArray[j][i];
        }
        result[i] = findMax(column, matrix->height);
    }
    free(column);
    return result;
}

int* findColumnMins(MATRIX* matrix) {
    int* result = malloc(sizeof(int) * matrix->width);
    int* column = malloc(sizeof(int) * matrix->height);
    int i;
    for (i = 0; i < matrix->width; ++i) {
        int j;
        for (j = 0; j < matrix->height; ++j) {
            column[j] = matrix->multiArray[j][i];
        }
        result[i] = findMin(column, matrix->height);
    }
    free(column);
    return result;
}


void pad(MATRIX* matrix, int incHeight, int incWidth, char* mode, FILE *file) {

    if (strcmp(mode, "maximum") == 0) {
        int i;
        for (i = 0; i < matrix->height; ++i) {
            int max = findMax(matrix->multiArray[i], matrix->width);
            matrix->multiArray[i] = realloc(matrix->multiArray[i], sizeof(int) * (matrix->width + incWidth));
            int j;
            for (j = 0; j < incWidth; ++j) {
                matrix->multiArray[i][j + matrix->width] = max;
            }
        }
        matrix->width = matrix->width + incWidth;
        int* maxColumns = findColumnMaxes(matrix);
        matrix->multiArray = realloc(matrix->multiArray, sizeof(int*) * (matrix->height + incHeight));
        int k;
        for (k = 0; k < incHeight; k++) {
            matrix->multiArray[k + matrix->height] = deepCopyArray(maxColumns, matrix->width);
        }
        matrix->height = matrix->height + incHeight;
        fprintf(file,"matrix paded %s %d %d\n", matrix->name, matrix->height, matrix->width);
        displayMatrix(matrix, file);
        free(maxColumns);
    } else if (strcmp(mode, "minimum") == 0) {
        int i;
        for (i = 0; i < matrix->height; ++i) {
            int min = findMin(matrix->multiArray[i], matrix->width);
            matrix->multiArray[i] = realloc(matrix->multiArray[i], sizeof(int) * (matrix->width + incWidth));
            int j;
            for (j = 0; j < incWidth; ++j) {
                matrix->multiArray[i][j + matrix->width] = min;
            }
        }
        matrix->width = matrix->width + incWidth;
        int* minColumns = findColumnMins(matrix);
        matrix->multiArray = realloc(matrix->multiArray, sizeof(int*) * (matrix->height + incHeight));
        int k;
        for (k = 0; k < incHeight; k++) {
            matrix->multiArray[k + matrix->height] = deepCopyArray(minColumns, matrix->width);
        }
        matrix->height = matrix->height + incHeight;
        fprintf(file, "matrix paded %s %d %d\n", matrix->name, matrix->height, matrix->width);
        displayMatrix(matrix, file);
        free(minColumns);
    } else {
        fprintf(file, "Wrong parameter!");
    }

}

void padVal(MATRIX* matrix, int incHeight, int incWidth, int val, FILE *file) {
    int i;
    if (!matrix) {fprintf(file, "error\n");return;}
    for (i = 0; i < matrix->height; ++i) {
        matrix->multiArray[i] = realloc(matrix->multiArray[i], sizeof(int) * (matrix->width + incWidth));
        int j;
        for (j = 0; j < incWidth; ++j) {
            matrix->multiArray[i][j + matrix->width] = val;
        }
    }
    matrix->width = matrix->width + incWidth;
    matrix->multiArray = realloc(matrix->multiArray, sizeof(int*) * (matrix->height + incHeight));
    int k;
    int j;
    for (k = 0; k < incHeight; ++k) {
        matrix->multiArray[k + matrix->height] = malloc(sizeof(int) * matrix->width);
        for (j = 0; j < matrix->width; ++j) {
            matrix->multiArray[k + matrix->height][j] = val;
        }
    }
    matrix->height = matrix->height + incHeight;
    fprintf(file, "matrix paded %s %d %d\n", matrix->name, matrix->height, matrix->width);
    displayMatrix(matrix, file);
}

VECTOR** vecSlice(VECTOR* vector, int start, int stop, char* name, VECTOR** vectors, int* totalCount, FILE *file) {

    if (start >= stop) {
        fprintf(file, "Error!");
        return vectors;
    }

    if (stop > vector->arraySize) {
        stop = vector->arraySize;
    }

    int* newArray = malloc(sizeof(int) * stop);
    int i;
    for (i = 0; i < stop - start; i++) {
        newArray[i] = vector->array[i + start];
    }
    VECTOR* slicedVector = initializeVector(name, stop - start, newArray);
    fprintf(file, "vector sliced %s %d\n", slicedVector->name, slicedVector->arraySize);
    displayVector(slicedVector->array, slicedVector->arraySize, file);
    return addVector(slicedVector, vectors, totalCount);
}

VECTOR** matSliceCol(MATRIX* matrix, int columnNumber, int start, int stop, char* name,  VECTOR** vectors, int* count, FILE *file) {
    if (columnNumber > matrix->width) {
        fprintf(file, "Error! Column number is greater than the width!");
        return vectors;
    }

    if (stop > matrix->height) {
        stop = matrix->height;
    }

    int* array = malloc(sizeof(int) * (stop - start));
    int i;
    for (i = 0; i < stop - start; ++i) {
        array[i] = matrix->multiArray[start + i][columnNumber];
    }

    VECTOR* slicedVector = initializeVector(name, stop - start, array);
    fprintf(file, "vector sliced %s %d\n", slicedVector->name, slicedVector->arraySize);
    displayVector(slicedVector->array, slicedVector->arraySize, file);
    return addVector(slicedVector, vectors, count);
}

VECTOR** matSliceRow(MATRIX* matrix, int rowNumber, int start, int stop, char* name,  VECTOR** vectors, int* count, FILE *file) {
    if (rowNumber > matrix->height) {
        fprintf(file, "Error! Number of rows is greater height of matrix!");
        return vectors;
    }

    if (stop > matrix->width)  {
        stop = matrix->width;
    }

    int* array = malloc(sizeof(int) * (stop - start));
    int i;
    for (i = 0; i < stop - start; ++i) {
        array[i] = matrix->multiArray[rowNumber][start + i];
    }
    VECTOR* slicedVector = initializeVector(name, stop - start, array);
    fprintf(file, "vector sliced %s %d\n", slicedVector->name, slicedVector->arraySize);
    displayVector(slicedVector->array, slicedVector->arraySize, file);
    return addVector(slicedVector, vectors, count);
}

MATRIX** matSlice(MATRIX *matrix, int startColumn, int endColumn, int startRow, int endRow, char *name, MATRIX **matrices,
                  int* totalCount, FILE *file) {
    if (endColumn > matrix->width || startColumn < 0 || endRow > matrix->height || startRow < 0) {
        fprintf(file, "Matrix size error!!!\n");
        return matrices;
    }
    int** multiArray = malloc(sizeof(int*) * (endRow - startRow));
    int i;
    for (i = 0; i < endRow - startRow; ++i) {
        multiArray[i] = malloc(sizeof(int) * (endColumn - startColumn));
        int j;
        for (j = 0; j < endColumn - startColumn; ++j) {
            multiArray[i][j] = matrix->multiArray[startRow + i][startColumn + j];
        }
    }
    MATRIX* newMatrix = initializeMatrix(name, endRow - startRow, endColumn - startColumn, multiArray);
    fprintf(file, "matrix sliced %s %d %d\n", newMatrix->name, newMatrix->height, newMatrix->width);
    displayMatrix(newMatrix, file);
    return addMatrix(newMatrix, matrices, totalCount);
}

int applyOperation(int first, int second, char* operation) {
    if (strcmp(operation, "add") == 0) {
        return first + second;
    } else if (strcmp(operation, "multiply") == 0) {
        return first * second;
    } else if (strcmp(operation, "subtract") == 0) {
        return first - second;
    }
    return rand();
}

void math(MATRIX* matrix1, MATRIX* matrix2, char* operation, FILE *file) {
    if (matrix1->height != matrix2->height && matrix1->width != matrix2->width) {
        fprintf(file, "error\n");
        return;
    }

    int i;
    for (i = 0; i < matrix1->height; ++i) {
        int j;
        for (j = 0; j < matrix1->width; ++j) {
            int value = applyOperation(matrix1->multiArray[i][j], matrix2->multiArray[i][j], operation);
            matrix1->multiArray[i][j] = value;
        }
    }
    fprintf(file, "%s %s %s\n", operation, matrix1->name, matrix2->name);
    displayMatrix(matrix1, file);
}

VECTOR* findVectorByName(VECTOR** vectors, int *totalCount, char* name) {
    int i;
    for (i = 0; i < *totalCount; ++i) {
        if (strcmp(vectors[i]->name, name) == 0) {
            return vectors[i];
        }
    }
    printf("Vector not found!\n");
    return NULL;
}

MATRIX* findMatrixByName(MATRIX** matrices, int *totalCount, char* name) {
    int i;
    for (i = 0; i < *totalCount; ++i) {
        if (strcmp(matrices[i]->name, name) == 0) {
            return matrices[i];
        }
    }
    printf("Matrix not found!\n");
    return NULL;
}
int main(int argc, char *argv[]) {
    char* ARRAYS_PATH = argv[1];
    char* COMMAND_PATH = argv[2];
    char* OUTPUT_NAME = argv[3];

    FILE *file;
    file = fopen(OUTPUT_NAME, "w");

    char* fileText = readAllCharacters(COMMAND_PATH);
    char** commandLines = charsToLines(fileText);

    VECTOR** vectors = malloc(0);
    MATRIX** matrices = malloc(0);

    int totalVectorCount = 0;
    int totalMatrixCount = 0;

    int i = 0;

    while (commandLines[i] != NULL) {
        char* command = strtok(commandLines[i], " ");

        if (strcmp(command, "veczeros") == 0) {
            char* name = strtok(NULL, " ");
            char* length = strtok(NULL, " ");
            vectors = vecZeros(name, length, vectors, &totalVectorCount, file);
        } else if (strcmp(command, "matzeros") == 0){
            char* name = strtok(NULL, " ");
            char* height = strtok(NULL, " ");
            char* width = strtok(NULL, " ");
            matrices = matZeros(name, height, width, matrices, &totalMatrixCount, file);
        } else if (strcmp(command, "vecread") == 0) {
            char* fileName = strtok(NULL, " ");
            vectors = vecRead(ARRAYS_PATH, fileName, vectors, &totalVectorCount, file);
        } else if (strcmp(command, "matread") == 0) {
            char *fileName = strtok(NULL, " ");
            matrices = matRead(ARRAYS_PATH, fileName, matrices, &totalMatrixCount, file);
        } else if (strcmp(command, "vecstack") == 0) {
            char* vector1 = strtok(NULL, " ");
            char* vector2 = strtok(NULL, " ");
            char* direction = strtok(NULL, " ");
            char* name = strtok(NULL, " ");
            matrices = vecStack(findVectorByName(vectors, &totalVectorCount, vector1),
                                findVectorByName(vectors, &totalVectorCount, vector2), direction, name, matrices,
                                &totalMatrixCount, file);
        } else if (strcmp(command, "matstack") == 0) {
            char* matrix1 = strtok(NULL, " ");
            char* matrix2 = strtok(NULL, " ");
            char* where = strtok(NULL, " ");
            matStack(findMatrixByName(matrices, &totalMatrixCount, matrix1),
                     findMatrixByName(matrices, &totalMatrixCount, matrix2), where, file);
        } else if (strcmp(command, "mvstack") == 0) {
            char* matrix = strtok(NULL, " ");
            char* vector = strtok(NULL, " ");
            char* where = strtok(NULL, " ");
            mvStack(findMatrixByName(matrices, &totalMatrixCount, matrix),
                    findVectorByName(vectors, &totalVectorCount, vector), where, file);
        } else if (strcmp(command, "pad") == 0) {
            char* matrix = strtok(NULL, " ");
            char* incHeight = strtok(NULL, " ");
            char* incWidth = strtok(NULL, " ");
            char* mode = strtok(NULL, " ");
            pad(findMatrixByName(matrices, &totalMatrixCount, matrix), atoi(incHeight), atoi(incWidth), mode, file);
        } else if (strcmp(command, "padval") == 0) {
            char* matrix = strtok(NULL, " ");
            char* incHeight = strtok(NULL, " ");
            char* incWidth = strtok(NULL, " ");
            char* val = strtok(NULL, " ");
            padVal(findMatrixByName(matrices, &totalMatrixCount, matrix), atoi(incHeight), atoi(incWidth), atoi(val), file);
        } else if (strcmp(command, "vecslice") == 0) {
            char* vector = strtok(NULL, " ");
            char* start = strtok(NULL, " ");
            char* stop = strtok(NULL, " ");
            char* name = strtok(NULL, " ");
            vectors = vecSlice(findVectorByName(vectors, &totalVectorCount, vector), atoi(start), atoi(stop), name,
                               vectors, &totalVectorCount, file);
        } else if (strcmp(command, "matslicecol") == 0) {
            char* matrix = strtok(NULL, " ");
            char* index = strtok(NULL, " ");
            char* start = strtok(NULL, " ");
            char* stop = strtok(NULL, " ");
            char* name = strtok(NULL, " ");
            vectors = matSliceCol(findMatrixByName(matrices, &totalMatrixCount, matrix), atoi(index), atoi(start), atoi(stop),
                                  name, vectors, &totalVectorCount, file);
        } else if (strcmp(command, "matslicerow") == 0) {
            char* matrix = strtok(NULL, " ");
            char* index = strtok(NULL, " ");
            char* start = strtok(NULL, " ");
            char* stop = strtok(NULL, " ");
            char* name = strtok(NULL, " ");
            vectors = matSliceRow(findMatrixByName(matrices, &totalMatrixCount, matrix), atoi(index), atoi(start), atoi(stop),
                                  name, vectors, &totalVectorCount, file);
        } else if (strcmp(command, "matslice") == 0) {
            char* matrix = strtok(NULL, " ");
            char* startColumn = strtok(NULL, " ");
            char* endColumn = strtok(NULL, " ");
            char* startRow = strtok(NULL, " ");
            char* endRow = strtok(NULL, " ");
            char* name = strtok(NULL, " ");
            matrices = matSlice(findMatrixByName(matrices, &totalMatrixCount, matrix), atoi(startColumn),
                                atoi(endColumn), atoi(startRow), atoi(endRow), name, matrices,
                                &totalMatrixCount, file);
        } else if (strcmp(command, "add") == 0) {
            char *matrix1 = strtok(NULL, " ");
            char *matrix2 = strtok(NULL, " ");
            math(findMatrixByName(matrices, &totalMatrixCount, matrix1),
                 findMatrixByName(matrices, &totalMatrixCount, matrix2), "add", file);
        } else if (strcmp(command, "multiply") == 0) {
            char *matrix1 = strtok(NULL, " ");
            char *matrix2 = strtok(NULL, " ");
            math(findMatrixByName(matrices, &totalMatrixCount, matrix1),
                 findMatrixByName(matrices, &totalMatrixCount, matrix2), "multiply", file);
        } else if (strcmp(command, "subtract") == 0) {
            char *matrix1 = strtok(NULL, " ");
            char *matrix2 = strtok(NULL, " ");
            math(findMatrixByName(matrices, &totalMatrixCount, matrix1),
                 findMatrixByName(matrices, &totalMatrixCount, matrix2), "subtract", file);
        } else {
            fprintf(file, "error\n");
        }
        i++;
    }
    free(fileText);
    free(commandLines);
    freeAllVectors(vectors, totalVectorCount);
    freeAllMatrices(matrices, totalMatrixCount);
    free(vectors);
    free(matrices);
    fclose(file);
    return 0;
}