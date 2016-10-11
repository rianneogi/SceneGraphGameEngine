#pragma once

const float CONST_BLOCKSIZE = 2.f;

struct Vector2i
{
	int x, y;
	Vector2i() : x(0), y(0) {}
	Vector2i(int a, int b) : x(a), y(b) {}
};

struct Vector3i
{
	int x, y, z;
	Vector3i() : x(0), y(0), z(0) {}
	Vector3i(int a, int b, int c) : x(a), y(b), z(c) {}
};

struct Vector2f
{
	float x, y;
	Vector2f() : x(0), y(0) {}
	Vector2f(float a, float b) : x(a), y(b) {}
};

struct Vector3f
{
	float x, y, z;
	Vector3f() : x(0), y(0), z(0) {}
	Vector3f(float a, float b, float c) : x(a), y(b), z(c) {}
	Vector3f(Vector2f a, float b) : x(a.x), y(a.y), z(b) {}
};
