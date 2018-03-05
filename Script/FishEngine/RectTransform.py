from . import Component, Rect, Vector2
import FishEngineInternal

class RectTransform(Component):
    ClassID = 224
    __slots__ = ('m_AnchorMin', 'm_AnchorMax', 'm_AnchoredPosition', 'm_SizeDelta', 'm_Pivot')
    def __init__(self):
        super().__init__()
        self.m_CachedPtr = FishEngineInternal.RectTransform()
        self.m_CachedPtr.SetObject(self)
        # self.m_AnchorMin = [0.5, 0]
        # self.m_AnchorMax =[0.5, 0]
        # self.m_AnchoredPosition = [0, 83]
        # self.m_SizeDelta = [160, 60]
        # self.m_Pivot = [160, 60]

    @property
    def rect(self)->Rect:
        pass

    @property
    def anchorMin(self)->Vector2:
        return Vector2.Wrap(self.m_CachedPtr.m_AnchorMin)
    @anchorMin.setter
    def anchorMin(self, value:Vector2):
        self.m_CachedPtr.m_AnchorMin.Set(value.x ,value.y)

    @property
    def anchorMax(self)->Vector2:
        return Vector2.Wrap(self.m_CachedPtr.m_AnchorMax)
    @anchorMax.setter
    def anchorMax(self, value:Vector2):
        self.m_CachedPtr.m_AnchorMax.Set(value.x ,value.y)

    @property
    def anchoredPosition(self)->Vector2:
        return Vector2.Wrap(self.m_CachedPtr.m_AnchoredPosition)
    @anchoredPosition.setter
    def anchoredPosition(self, value:Vector2):
        self.m_CachedPtr.m_AnchoredPosition.Set(value.x ,value.y)

    @property
    def sizeDelta(self)->Vector2:
        return Vector2.Wrap(self.m_CachedPtr.m_SizeDelta)
    @sizeDelta.setter
    def sizeDelta(self, value:Vector2):
        self.m_CachedPtr.m_SizeDelta.Set(value.x ,value.y)

    @property
    def pivot(self)->Vector2:
        return Vector2.Wrap(self.m_CachedPtr.m_Pivot)
    @pivot.setter
    def pivot(self, value:Vector2):
        self.m_CachedPtr.m_Pivot.Set(value.x ,value.y)