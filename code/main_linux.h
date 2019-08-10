
struct XState {
	Display *display;
	i32 rootWindow;
	i32 defaultScreen;
	Window window;
	
	Atom atomDeleteWindow;
	
	GLXContext glContext;
};

inline u8 GetKeyCode(XState *xState, u64 keySymbol) {
	return XKeysymToKeycode(xState->display, keySymbol);
}