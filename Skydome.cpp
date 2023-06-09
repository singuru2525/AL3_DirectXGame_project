#include "Skydome.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <cmath>
#include <cassert>

void Skydome::Initialize(Model* model) {

	assert(model);

	model_ = model;

	worldTransform_.Initialize();

};

void Skydome::Update(){

	worldTransform_.UpdateMatrix();

};

void Skydome::Draw(const ViewProjection& viewProjection) {

	model_->Draw(worldTransform_, viewProjection);

};