# https://stackoverflow.com/questions/21473076/pyyaml-and-unusual-tags
def removeUnityTagAlias(filepath):
    """
    Name:               removeUnityTagAlias()
    Description:        Loads a file object from a Unity textual scene file, which is in a pseudo YAML style, and strips the
                        parts that are not YAML 1.1 compliant. Then returns a string as a stream, which can be passed to PyYAML.
                        Essentially removes the "!u!" tag directive, class type and the "&" file ID directive. PyYAML seems to handle.
                        rest just fine after that.
    Returns:            String (YAML stream as string)  
    """
    result = str()
    sourceFile = open(filepath, 'r')
    objectCount = 0
    fileID2Index = {}

    for _,line in enumerate( sourceFile.readlines() ): 
        if line.startswith('--- !u!'):
            # TODO: --- !u!4 &2039011982 stripped
            fileID = line.split(' ')[2].strip()
            result += '--- ' + fileID + '\n'   # remove the tag, but keep file ID, also remove keyword 'stripped'
            fileID = fileID[1:]
            fileID2Index[int(fileID)] = objectCount
            objectCount += 1
        else:
            # Just copy the contents...
            result += line
            
    sourceFile.close()
    return (result, fileID2Index)