import re
import os

shader_include_root = '/Users/yushroom/program/FishEngine/Engine/Shaders/include'

def Print(*args, **kw):
    # print(args, kw)
    pass

# https://gist.github.com/ChunMinChang/88bfa5842396c1fbbc5b
def removeComments(text):
    """ remove c-style comments.
        text: blob of text with comments (can include newlines)
        returns: text with comments removed
    """
    pattern = r"""
                            ##  --------- COMMENT ---------
           //.*?$           ##  Start of // .... comment
         |                  ##
           /\*              ##  Start of /* ... */ comment
           [^*]*\*+         ##  Non-* followed by 1-or-more *'s
           (                ##
             [^/*][^*]*\*+  ##
           )*               ##  0-or-more things which don't start with /
                            ##    but do end with '*'
           /                ##  End of /* ... */ comment
         |                  ##  -OR-  various things which aren't comments:
           (                ##
                            ##  ------ " ... " STRING ------
             "              ##  Start of " ... " string
             (              ##
               \\.          ##  Escaped char
             |              ##  -OR-
               [^"\\]       ##  Non "\ characters
             )*             ##
             "              ##  End of " ... " string
           |                ##  -OR-
                            ##
                            ##  ------ ' ... ' STRING ------
             '              ##  Start of ' ... ' string
             (              ##
               \\.          ##  Escaped char
             |              ##  -OR-
               [^'\\]       ##  Non '\ characters
             )*             ##
             '              ##  End of ' ... ' string
           |                ##  -OR-
                            ##
                            ##  ------ ANYTHING ELSE -------
             .              ##  Anything other char
             [^/"'\\]*      ##  Chars which doesn't start a comment, string
           )                ##    or escape
    """
    regex = re.compile(pattern, re.VERBOSE|re.MULTILINE|re.DOTALL)
    noncomments = [m.group(2) for m in regex.finditer(text) if m.group(2)]
    return "".join(noncomments)

def commentRemover(text):
    def replacer(match):
        s = match.group(0)
        if s.startswith('/'):
            return " " # note: a space and not an empty string
        else:
            return s
    pattern = re.compile(
        r'//.*?$|/\*.*?\*/|\'(?:\\.|[^\\\'])*\'|"(?:\\.|[^\\"])*"',
        re.DOTALL | re.MULTILINE
    )
    return re.sub(pattern, replacer, text)

done = {}

def PreprocessShaderFromFile(shader_file_path):
    Print(shader_file_path)
    if shader_file_path in done:
        # return '\n'   # do not include it again
        return done[shader_file_path]
    shader_str = ''
    with open(shader_file_path) as f:
        lines = f.readlines()
        # for l in lines:
        #     l = l.strip()
        #     if not l.startswith(r"//"):
        # lines = [l for l in lines if not l.strip().startswith(r'//')]
        shader_str = ''.join( lines )

    # print(re.search(r'^\s*@vertex', r"// @vertex"))
    # print(re.search(r'^\s*@vertex', r"@vertex"))
    # print(re.search(r'^\s*@vertex', r"  @vertex"))

    Print("1. remove comments:")
    # Print(commentRemover(shader_str))
    shader_str = commentRemover(shader_str)

    # print(shader_str)
    has_vert = '@vertex' in shader_str
    has_frag = '@fragment' in shader_str
    has_geometry = '@geometry' in shader_str

    Print('2. replace @vertex/@fragment/@geometry')
    def Replace(shader_str, tag, define_begin, define_end="#endif"):
        pos = shader_str.find(tag)
        begin = pos
        count = 1
        left_brace_pos = shader_str.find('{', pos)
        pos = left_brace_pos+1
        while count > 0:
            pos1 = shader_str.find('{', pos)
            pos2 = shader_str.find('}', pos)
            Print(count, pos1, pos2)
            # assert(pos1 != -1)
            # assert(pos2 != -1)
            if pos1 == -1:
                pos1 = len(shader_str)
            if pos2 == -1:
                pos2 = len(shader_str)
            if pos1 < pos2:
                # print('{')
                count += 1
                pos = pos1+1
            else:
                # print('}')
                count -= 1
                pos = pos2+1
        end = pos-1
        # end = shader_str.find('}', pos)
        Print(shader_str[begin:end+1])
        shader_str = shader_str[0:begin] + define_begin + shader_str[left_brace_pos+1:end] + define_end + shader_str[end+1:]
        # print(shader_str)
        return shader_str

    if has_vert:
        shader_str = Replace(shader_str, "@vertex", "#ifdef VERTEX")
    if has_geometry:
        shader_str = Replace(shader_str, "@geometry", "#ifdef GEOMETRY")
    if has_frag:
        shader_str = Replace(shader_str, "@fragment", "#ifdef FRAGMENT")


    Print("3. replace include files")
    # https://stackoverflow.com/questions/21047053/python-regex-matching-pair-of-quotes-or
    pattern = re.compile(r"#\s*include\s*(?:<.*?>|\".*?\")")

    include_directives = pattern.findall(shader_str)
    Print(include_directives)
    for l in include_directives:
        sys_include = re.compile(r'#\s*include\s*<(.*)>')
        m = sys_include.match(l)
        # print(m.group(0))
        # print(m.group(1))
        header_path = os.path.join(shader_include_root, m.group(1))
        s = PreprocessShaderFromFile(header_path)
        done[header_path] = s
        shader_str = shader_str.replace(l, s, 1)   # replace the first one

    return shader_str

if __name__ == "__main__":
    shader_path = '/Users/yushroom/program/FishEngine/Engine/Shaders/GatherScreenSpaceShadow.shader'
    result = PreprocessShaderFromFile(shader_path)
    print(result)
    # s = "12345678"
    # s[0:5] = "[0-5]"