#include "skeletalanimation.h"


void SkeletalAnimation::addKeyFrame(std::shared_ptr<KeyFrame> keyframe) {
    _keyFrames.push_back(keyframe);
    m_length = keyframe->getTimeStamp();
}

SkeletalAnimation::SkeletalAnimation() {
}



float SkeletalAnimation::getLength() {
    return m_length;
}

std::tuple<KeyFrame*, KeyFrame*, float> SkeletalAnimation::getPreviousAndNextKeyFrames(float t) {
    if (t < 0 || t > m_length) {
        return std::make_tuple(nullptr, nullptr, 0.0f);
    }
    for (int i = 1; i < _keyFrames.size(); ++i) {
        if (_keyFrames[i]->getTimeStamp() > t) {
            float progression = (t - _keyFrames[i-1]->getTimeStamp()) / (_keyFrames[i]->getTimeStamp() - _keyFrames[i-1]->getTimeStamp());
            return std::make_tuple(_keyFrames[i-1].get(), _keyFrames[i].get(), progression);
        }
    }
    // if we get here, we are in a loop
    float progression = (t - _keyFrames.back()->getTimeStamp()) / (m_length - _keyFrames.back()->getTimeStamp());
    return std::make_tuple(_keyFrames.back().get(), _keyFrames.front().get(), progression);

}
