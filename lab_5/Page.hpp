class Page {
    public:
        // Constructors
        Page();
        Page(unsigned int owner) : owner(owner) {}
        Page(unsigned int owner, unsigned int frame) : owner(owner), frame(frame) {}
        // Public data members
        bool in_memory = false;
        unsigned int owner = 0;
        unsigned int frame = 0;
};
