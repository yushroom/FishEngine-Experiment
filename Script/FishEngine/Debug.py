import traceback

class Debug:
    @staticmethod
    def Log(*objs):
        print(*objs)
        for line in traceback.format_stack(limit=5):
            print(line.strip())