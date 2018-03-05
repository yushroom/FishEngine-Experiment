from FishEngine import Script
class Text(Script):
    def __int__(self):
        self.m_FontData = None
        self.m_Text:str = ""

    @property
    def text(self)->str:
        return self.m_Text
    @text.setter
    def text(self, value:str):
        self.m_Text = value