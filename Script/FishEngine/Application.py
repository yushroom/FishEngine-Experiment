class Applicatoin:

    @property
    def dataPath(self):
        """
        Contains the path to the game data folder (Read Only). \n
        The value depends on which platform you are running on: \n
        - Unity Editor: <path to project folder>/Assets
        """
        return ''