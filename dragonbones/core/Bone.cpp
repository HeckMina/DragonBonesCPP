#include "Bone.h"

NAME_SPACE_DRAGON_BONES_BEGIN
bool Bone::sortState(const TimelineState *a, const TimelineState *b)
{
    return a->getLayer() < b->getLayer();
}

Slot *Bone::getSlot() const
{
    return _slotList.empty() ? nullptr : _slotList.front();
}

const std::vector<Slot *> &Bone::getSlots() const
{
    return _slotList;
}

const std::vector<Bone *> &Bone::getBones() const
{
    return _boneList;
}

void Bone::setVisible(bool visible)
{
    if (_visible != visible)
    {
        _visible = visible;
        for (size_t i = 0, l = _slotList.size(); i < l; ++i)
        {
            _slotList[i]->updateDisplayVisible(_visible);
        }
    }
}

void Bone::setArmature(Armature *armature)
{
    Object::setArmature(armature);
    for (size_t i = 0, l = _boneList.size(); i < l; ++i)
    {
        _boneList[i]->setArmature(armature);
    }
    for (size_t i = 0, l = _slotList.size(); i < l; ++i)
    {
        _slotList[i]->setArmature(armature);
    }
}

Bone::Bone()
{
    _isColorChanged = false;
    _needUpdate = 2;
    _tween.scaleX = _tween.scaleY = 0.f;
    inheritRotation = true;
    inheritScale = false;
}
Bone::~Bone()
{
    dispose();
}
void Bone::dispose()
{
    Object::dispose();
    for (size_t i = 0, l = _boneList.size(); i < l; ++i)
    {
        if (_boneList[i])
        {
            _boneList[i]->dispose();
            delete _boneList[i];
        }
    }
    for (size_t i = 0, l = _slotList.size(); i < l; ++i)
    {
        if (_slotList[i])
        {
            _slotList[i]->dispose();
            delete _slotList[i];
        }
    }
    _boneList.clear();
    _slotList.clear();
    _timelineStateList.clear();
}

void Bone::invalidUpdate()
{
    _needUpdate = 2;
}

bool Bone::contains(const Object *object) const
{
    if (!object)
    {
        // throw
    }
    if (object == this)
    {
        return false;
    }
    const Object *ancestor = object;
    while (!(ancestor == this || ancestor == nullptr))
    {
        ancestor = ancestor->getParent();
    }
    return ancestor == this;
}

void Bone::addChild(Object *object)
{
    if (!object)
    {
        // throw
    }
    Bone *bone = dynamic_cast<Bone *>(object);
    Slot *slot = dynamic_cast<Slot *>(object);
    if (object == this || (bone && bone->contains(this)))
    {
        throw std::invalid_argument("An Bone cannot be added as a child to itself or one of its children (or children's children, etc.)");
    }
    if (object && object->getParent())
    {
        object->getParent()->removeChild(object);
    }
    if (bone)
    {
        _boneList.push_back(bone);
        bone->setParent(this);
        bone->setArmature(_armature);
    }
    else if (slot)
    {
        _slotList.push_back(slot);
        slot->setParent(this);
        slot->setArmature(_armature);
    }
}

void Bone::removeChild(Object *object)
{
    if (!object)
    {
        // throw
    }
    Bone *bone = dynamic_cast<Bone *>(object);
    Slot *slot = dynamic_cast<Slot *>(object);
    if (bone)
    {
        auto iterator = std::find(_boneList.begin(), _boneList.end(), bone);
        if (iterator != _boneList.end())
        {
            _boneList.erase(iterator);
            bone->setParent(nullptr);
            bone->setArmature(nullptr);
        }
        else
        {
            // throw
        }
    }
    else if (slot)
    {
        auto iterator = std::find(_slotList.begin(), _slotList.end(), slot);
        if (iterator != _slotList.end())
        {
            _slotList.erase(iterator);
            slot->setParent(nullptr);
            slot->setArmature(nullptr);
        }
        else
        {
            // throw
        }
    }
}

void Bone::update(bool needUpdate)
{
    _needUpdate --;
    if (needUpdate || _needUpdate > 0 || (_parent && _parent->_needUpdate > 0))
    {
        _needUpdate = 1;
    }
    else
    {
        return;
    }
    blendingTimeline();
    global.scaleX = (origin.scaleX + _tween.scaleX) * offset.scaleX;
    global.scaleY = (origin.scaleY + _tween.scaleY) * offset.scaleY;
    if (_parent)
    {
        const float x = origin.x + offset.x + _tween.x;
        const float y = origin.y + offset.y + _tween.y;
        const Matrix &parentMatrix = _parent->globalTransformMatrix;
        globalTransformMatrix.tx = global.x = parentMatrix.a * x + parentMatrix.c * y + parentMatrix.tx;
        globalTransformMatrix.ty = global.y = parentMatrix.d * y + parentMatrix.b * x + parentMatrix.ty;
        if (inheritRotation)
        {
            global.skewX = origin.skewX + offset.skewX + _tween.skewX + _parent->global.skewX;
            global.skewY = origin.skewY + offset.skewY + _tween.skewY + _parent->global.skewY;
        }
        else
        {
            global.skewX = origin.skewX + offset.skewX + _tween.skewX;
            global.skewY = origin.skewY + offset.skewY + _tween.skewY;
        }
        if (inheritScale)
        {
            global.scaleX *= _parent->global.scaleX;
            global.scaleY *= _parent->global.scaleY;
        }
    }
    else
    {
        globalTransformMatrix.tx = global.x = origin.x + offset.x + _tween.x;
        globalTransformMatrix.ty = global.y = origin.y + offset.y + _tween.y;
        global.skewX = origin.skewX + offset.skewX + _tween.skewX;
        global.skewY = origin.skewY + offset.skewY + _tween.skewY;
    }
    /*
    globalTransformMatrix.a = global.scaleX * cos(global.skewY);
    globalTransformMatrix.b = global.scaleX * sin(global.skewY);
    globalTransformMatrix.c = -global.scaleY * sin(global.skewX);
    globalTransformMatrix.d = global.scaleY * cos(global.skewX);
    */
    globalTransformMatrix.a = offset.scaleX * cos(global.skewY);
    globalTransformMatrix.b = offset.scaleX * sin(global.skewY);
    globalTransformMatrix.c = -offset.scaleY * sin(global.skewX);
    globalTransformMatrix.d = offset.scaleY * cos(global.skewX);
}

void Bone::updateColor(
    int aOffset,
    int rOffset,
    int gOffset,
    int bOffset,
    float aMultiplier,
    float rMultiplier,
    float gMultiplier,
    float bMultiplier,
    bool colorChanged
)
{
    for (size_t i = 0, l = _slotList.size(); i < l; ++i)
    {
        _slotList[i]->updateDisplayColor(
            aOffset, rOffset, gOffset, bOffset,
            aMultiplier, rMultiplier, gMultiplier, bMultiplier
        );
    }
    _isColorChanged = colorChanged;
}

void Bone::hideSlots()
{
    for (size_t i = 0, l = _slotList.size(); i < l; ++i)
    {
        _slotList[i]->changeDisplay(-1);
    }
}

void Bone::arriveAtFrame(const TransformFrame *frame, const TimelineState *timelineState, AnimationState *animationState, const bool isCross)
{
    // TODO:
    const bool displayControl =
        animationState->displayControl &&
        (displayController.empty() || displayController == animationState->name);
    if (displayControl)
    {
        const int displayIndex = frame->displayIndex;
        for (size_t i = 0, l = _slotList.size(); i < l; ++i)
        {
            Slot *slot = _slotList[i];
            slot->changeDisplay(displayIndex);
            slot->updateDisplayVisible(frame->visible);
            if (displayIndex >= 0)
            {
                if (frame->zOrder != slot->_tweenZOrder)
                {
                    //slot->_tweenZOrder = frame->zOrder;
                    //_armature->_slotsZOrderChanged = true;
                }
            }
        }
        if (!frame->event.empty() && _armature->_eventDispatcher->hasEvent(EventData::EventDataType::BONE_FRAME_EVENT))
        {
            EventData *eventData = new EventData(EventData::EventDataType::BONE_FRAME_EVENT, _armature);
            eventData->bone = this;
            eventData->animationState = animationState;
            eventData->frameLabel = frame->event;
            _armature->_eventDataList.push_back(eventData);
        }
        if (!frame->sound.empty() && Armature::soundEventDispatcher && Armature::soundEventDispatcher->hasEvent(EventData::EventDataType::SOUND))
        {
            EventData *eventData = new EventData(EventData::EventDataType::SOUND, _armature);
            eventData->bone = this;
            eventData->animationState = animationState;
            eventData->sound = frame->sound;
            Armature::soundEventDispatcher->dispatchEvent(eventData);
        }
        if (!frame->action.empty())
        {
            for (size_t i = 0, l = _slotList.size(); i < l; ++i)
            {
                if (_slotList[i]->_childArmature)
                {
                    _slotList[i]->_childArmature->_animation->gotoAndPlay(frame->action);
                }
            }
        }
    }
}

void Bone::addState(TimelineState *timelineState)
{
    auto iterator = std::find(_timelineStateList.cbegin(), _timelineStateList.cend(), timelineState);
    if (iterator == _timelineStateList.cend())
    {
        _timelineStateList.push_back(timelineState);
        std::sort(_timelineStateList.begin() , _timelineStateList.end() , sortState);
    }
}

void Bone::removeState(TimelineState *timelineState)
{
    auto iterator = std::find(_timelineStateList.begin(), _timelineStateList.end(), timelineState);
    if (iterator != _timelineStateList.end())
    {
        _timelineStateList.erase(iterator);
    }
}

void Bone::blendingTimeline()
{
    size_t i = _timelineStateList.size();
    if (i == 1)
    {
        const TimelineState *timelineState = _timelineStateList[0];
        const Transform &transform = timelineState->_transform;
        const Point &pivot = timelineState->_pivot;
        const float weight = timelineState->getWeight();
        _tween.x = transform.x * weight;
        _tween.y = transform.y * weight;
        _tween.skewX = transform.skewX * weight;
        _tween.skewY = transform.skewY * weight;
        _tween.scaleX = transform.scaleX * weight;
        _tween.scaleY = transform.scaleY * weight;
        _tweenPivot.x = pivot.x * weight;
        _tweenPivot.y = pivot.y * weight;
    }
    else if (i > 1)
    {
        int prevLayer = _timelineStateList[i - 1]->getLayer();
        int currentLayer = 0;
        float weigthLeft = 1.f;
        float layerTotalWeight = 0.f;
        float x = 0.f;
        float y = 0.f;
        float skewX = 0.f;
        float skewY = 0.f;
        float scaleX = 0.f;
        float scaleY = 0.f;
        float pivotX = 0.f;
        float pivotY = 0.f;
        while (i--)
        {
            const TimelineState *timelineState = _timelineStateList[i];
            currentLayer = timelineState->getLayer();
            if (prevLayer != currentLayer)
            {
                if (layerTotalWeight >= weigthLeft)
                {
                    break;
                }
                else
                {
                    weigthLeft -= layerTotalWeight;
                }
            }
            prevLayer = currentLayer;
            const float weight = timelineState->getWeight() * weigthLeft;
            if (weight && timelineState->_blendEnabled)
            {
                const Transform &transform = timelineState->_transform;
                const Point &pivot = timelineState->_pivot;
                x += transform.x * weight;
                y += transform.y * weight;
                skewX += transform.skewX * weight;
                skewY += transform.skewY * weight;
                scaleX += transform.scaleX * weight;
                scaleY += transform.scaleY * weight;
                pivotX += pivot.x * weight;
                pivotY += pivot.y * weight;
                layerTotalWeight += weight;
            }
        }
        _tween.x = x;
        _tween.y = y;
        _tween.skewX = skewX;
        _tween.skewY = skewY;
        _tween.scaleX = scaleX;
        _tween.scaleY = scaleY;
        _tweenPivot.x = pivotX;
        _tweenPivot.y = pivotY;
    }
}
NAME_SPACE_DRAGON_BONES_END