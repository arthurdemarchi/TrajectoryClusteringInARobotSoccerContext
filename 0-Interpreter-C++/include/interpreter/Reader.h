// reader.h
#ifndef INTERPRETER_READER
#define INTERPRETER_READER

class Reader{
    private:
        //Attributes
        Data data;
        Path path;

        //Methods
        updateData();

    public:
        setPath(char *path);
        getPath();
}

#endif