CXX = clang++
# Use '-O3 -flto' in final iteration
CXXFLAGS = -std=c++20 -I/Users/vova/Code/C++/libraries/FTXUI/include
LDLFLAGS = -L/Users/vova/Code/C++/libraries/FTXUI/build -lftxui-component -lftxui-dom -lftxui-screen

%: %.cpp
	@$(CXX) $(CXXFLAGS) $< -o $@ $(LDLFLAGS)