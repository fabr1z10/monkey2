#include "skeletalmodel.h"
#include <glm/gtx/transform.hpp>
#include <iostream>


#include "skeletalrenderer.h"

#include "../game.h"
#include <pybind11/stl.h>


void SkeletalModel::addAnimation(const std::string& animId, std::shared_ptr<SkeletalAnimation> anim) {
    _animations[animId] = anim;

}
void SkeletalModel::setMesh(int jointId, std::shared_ptr<Mesh> mesh) {
    _jointInfos[jointId].mesh = mesh;
}

int SkeletalModel::addJoint(const std::string& name, int parent, IVec2 position) {
    JointInfo info;
    info.id = _jointInfos.size();
    info.parent = parent;
    info.mesh = nullptr;
    JointTransform tr;
    tr.scale = glm::vec3(1.f);
    tr.translation = glm::vec3(position.x, position.y, 0.0f);
    info.restTransform = tr;
    info.weightIndex = glm::ivec3(info.id, parent, 0);
    _jointInfos.push_back(info);
    _jointInfos[parent].children.push_back(info.id);
    _jointNameToId[name] = info.id;
    return info.id;
}

void SkeletalModel::init() {
    std::unordered_map<int, JointTransform> p;
    _invRestTransforms2 = std::vector<glm::mat4>(_jointInfos.size());
    for (size_t i = 0; i < _invRestTransforms2.size(); ++i) {
        _invRestTransforms2[i] = glm::mat4(1.0f);
    }
    _restTransforms2 = calculateCurrentPose(p);
    for (size_t i = 0; i <_invRestTransforms2.size(); ++i) {
        _restTransforms2[i][3][2] = _jointInfos[i].z;
        _invRestTransforms2[i] = glm::inverse(_restTransforms2[i]);
        //m_invRestTransforms2[i][3][2] = -m_jointInfos[i].z;
    }
}

SkeletalModel::SkeletalModel(int camId) : IModel(), _currentAnimation(nullptr), _camId(camId), _animationTime(0.f) {
	_cam = Game::instance().getRoom()->getCamera(_camId);
	_shaderId = 70;
	JointInfo info(0, -1, "root", {0,0,0});
    _jointInfos.push_back(info);
    _jointNameToId["root"] = 0;
    auto& game = Game::instance();
    if (!game.hasShader(_shaderId)) {
        auto* shader = Game::instance().getShader(_shaderId);
        game.getRoom()->addShader(shader);
    }
	_shader = Game::instance().getShader(_shaderId);
	_modelLocation = glGetUniformLocation(_shader->getProgId(), "model");
	_viewLocation = glGetUniformLocation(_shader->getProgId(), "view");
	_projLocation = glGetUniformLocation(_shader->getProgId(), "projection");
	_boneLocation = glGetUniformLocation(_shader->getProgId(), "Bone");
	_l2mLocation = glGetUniformLocation(_shader->getProgId(), "local_to_model");
	_weightIndexLocation = glGetUniformLocation(_shader->getProgId(), "weightIndex");
	_zLocation = glGetUniformLocation(_shader->getProgId(), "z");

}

//std::shared_ptr<IRenderer> SkeletalModel::getRenderer(int batchId) {
//    return std::make_shared<SkeletalRenderer>(this);
//}




std::vector<glm::mat4> SkeletalModel::calculateCurrentPose(std::unordered_map<int, JointTransform>& pose) {
    std::vector<glm::mat4> result(_jointInfos.size());
    std::list<std::pair<int, glm::mat4>> joints;
    joints.emplace_back(0, glm::mat4(1.0));
    while (!joints.empty()) {
        auto current = joints.front();
        joints.pop_front();
        const auto& joint = _jointInfos[current.first];
        auto localTransform = joint.restTransform;
        // TODO apply local transform
        auto i = pose.find(current.first);
        if (i != pose.end()) {
            localTransform += i->second;
        }
        auto localMat = localTransform.getLocalTransform();
        auto modelMat = current.second * localMat * glm::scale(glm::vec3(joint.scale));
        result[current.first] = modelMat * _invRestTransforms2[current.first];
        // TODO convert to global mat
        for (const auto &child : joint.children) {
            joints.emplace_back(child, modelMat);
        }
    }
    return result;

}

const glm::mat4& SkeletalModel::getRestTransform(int id) const {
    return _restTransforms2[id];
}

glm::ivec3 SkeletalModel::getWeightIndex(int id) const {
    return _jointInfos[id].weightIndex;
}

SkeletalAnimation * SkeletalModel::getAnimation(const std::string &id) {
    auto it = _animations.find(id);
    if (it == _animations.end())
        return nullptr;
    return it->second.get();
}

//void SkeletalModel::computeOffset() {
//    m_offsetPoints.clear();
//    for (const auto& p : m_offsetPointIds) {
//        int jointId = m_jointNameToId.at(p.first);
//        if (_jointInfos[jointId].mesh == nullptr) {
//            continue;
//        }
//        auto kp = _jointInfos[jointId].mesh->getKeyPoint(p.second);
//        auto mp = m_restTransforms2[jointId] * glm::vec4(kp.x, kp.y, 0.0f, 1.0f);
//        m_offsetPoints.emplace_back(jointId, glm::vec3(mp.x, mp.y, 0.0f));
//    }
//}

//std::shared_ptr<Shape> SkeletalModel::getShape (const std::string& anim) const {
//    const auto& it = m_animShapes.find(anim);
//    if (it == m_animShapes.end())
//        return nullptr;
//    return m_shapes[it->second[0]];
//}
//
//std::shared_ptr<Shape> SkeletalModel::getShapeCast(const std::string &animId, float t) const {
//    const auto& shapes = m_animShapes.find(animId);
//    if (shapes == m_animShapes.end() || shapes->second.size() <= 1) {
//        return nullptr;
//    }
//    auto anim = m_animations.at(animId);
//    auto attackId = anim->getAttack(t);
//    if (attackId == -1 || shapes->second.size() < attackId + 2) {
//        return nullptr;
//    }
//    return m_shapes[shapes->second[attackId+1]];
//
//}

//
//
//bool SkeletalModel::hasCollision(const std::string & anim) const {
//    return m_animShapes.count(anim) > 0;
//
//}

/*
std::shared_ptr<Model> SkeletalModel::generateDebugModel() {
    std::vector<float> vertices;
    std::vector<unsigned> elements;
    unsigned u{0};
    glm::vec4 color(1.f);
    auto model = std::make_shared<Model>(ShaderType::SHADER_COLOR, GL_LINES);

    // every shape is associated an offset and a count
    auto lambda = [&] (const Bounds& b) {
        vertices.insert(vertices.end(), {b.min.x, b.min.y, 0.0f, color.r, color.g, color.b, color.a});
        vertices.insert(vertices.end(), {b.max.x, b.min.y, 0.0f, color.r, color.g, color.b, color.a});
        vertices.insert(vertices.end(), {b.max.x, b.max.y, 0.0f, color.r, color.g, color.b, color.a});
        vertices.insert(vertices.end(), {b.min.x, b.max.y, 0.0f, color.r, color.g, color.b, color.a});
        elements.insert(elements.end(), {u, u+1, u+1, u+2, u+2, u+3, u+3, u});
        u += 4;
    };

    int ishape{0};
    for (const auto& s : m_shapes) {
        unsigned offset = elements.size();
        if (s->getShapeType() == ShapeType::COMPOUND) {
            auto* cs = static_cast<CompoundShape*>(s.get());
            for (const auto& t : cs->getShapes()) {
                lambda(t->getBounds());
            }
        } else {
            lambda(s->getBounds());
        }
        m_shapeInfo.emplace_back(offset, elements.size());
    }
    model->generateBuffers(vertices, elements);

    return model;
    //return std::make_shared<RawModel>(ShaderType::SHADER_COLOR, GL_LINES, vertices, elements);

}

std::pair<int, int> SkeletalModel::getDebugShape(const std::string &anim, int n) {
    const auto& it = m_animShapes.find(anim);
    if (it == m_animShapes.end() || n >= it->second.size())
        return std::make_pair(-1, -1);

    auto shapeId = it->second[n];
    return m_shapeInfo[shapeId];
}*/


void SkeletalModel::updateImpl() {
//	if (!_started) {
//		return;
//	}
	auto dt = 1./60.;
	std::unordered_map<int, JointTransform> pose;
	if (_currentAnimation != nullptr) {
		_animationTime += static_cast<float>(dt);
		if (_animationTime >= _currentAnimation->getLength()) {
			//m_complete = true;
			//if (_currentAnimation->loop()) {
			_animationTime = fmod(_animationTime, _currentAnimation->getLength());
			//}  else {
			//    _animationTime = _currentAnimation->getLength() - 0.001f;
			//}
		}
		// compute current pose
		auto frames = _currentAnimation->getPreviousAndNextKeyFrames(_animationTime);
		pose = interpolatePoses(std::get<0>(frames), std::get<1>(frames), std::get<2>(frames));
	}

	_bones = calculateCurrentPose(pose);
	//draw();
	// apply offset
//    const auto& offsetPoints = _skeletalModel->getOffsetPoints();
//    if (!offsetPoints.empty()) {
//        glm::vec3 offset(0.0f);
//        //std::cout << "no of offset points: " << offsetPoints.size() << "\n";
//        for (const auto &a : offsetPoints) {
//            // find coordinates of offset pointg
//            glm::vec4 p = _bones[a.first] * glm::vec4(a.second, 1.0f);
//            offset.y = std::max(-p.y, offset.y);
//        }
//        //std::cerr << offset.y << "\n";
//        setTransform(glm::translate(offset));
//    }
}


std::unordered_map<int, JointTransform> SkeletalModel::interpolatePoses(
		KeyFrame* previousFrame, KeyFrame* nextFrame, float progression) {
	std::unordered_map<int, JointTransform> currentPose;
	const auto& nf = nextFrame->getJointKeyFrames();
	for (const auto& p : previousFrame->getJointKeyFrames()) {
		// the model might not have this joint ... in this case nothing to do
		int jointId = getJointId(p.first);
		if (jointId == -1) {
			continue;
		}

		// previousTransform is p.second
		JointTransform nextTransform = nf.at(p.first);
		//JointTransform currentTransform = m_model->getRestTransform(p.first);
		auto localTransform = JointTransform::interpolate(p.second, nextTransform, progression);
		//currentTransform += JointTransform::interpolate(p.second, nextTransform, progression);
		//currentTransform.z = 0;
		//std::cout << m_animationTime << " . " << currentTransform.alpha << "\n";
		currentPose.insert(std::make_pair(jointId, localTransform));
	}
	return currentPose;
}

void SkeletalModel::draw() {
	auto w = _node->getWorldMatrix();
	auto pm = _cam->getProjectionMatrix();
	auto vm = _cam->getViewMatrix();
	glUniformMatrix4fv(_modelLocation, 1, false, &w[0][0]);
	glUniformMatrix4fv(_viewLocation, 1, false, &vm[0][0]);
	glUniformMatrix4fv(_projLocation, 1, false, &pm[0][0]);

	glUniformMatrix4fv(_boneLocation, _bones.size(), false, &_bones[0][0][0]);

			//for (const auto& model : _skeletalModel->getModels()) {
	for (size_t n = 0; n < getJointCount(); ++n) {
		const auto &jinfo = getJointInfo(n);
		auto restTransform = getRestTransform(n);

		auto weightIndices = jinfo.weightIndex;// m_skeletalModel->getWeightIndex(n++);
		glUniformMatrix4fv(_l2mLocation, 1, false, &restTransform[0][0]);
		glUniform3iv(_weightIndexLocation, 1, &weightIndices[0]);
		glUniform1f(_zLocation, jinfo.z);
		if (jinfo.mesh != nullptr) {
			jinfo.mesh->draw(_shader);
		}
	}

}

void SkeletalModel::setAnimation(const std::string & id) {
	_currentAnimation = getAnimation(id);
}