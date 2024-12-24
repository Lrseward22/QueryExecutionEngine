CXX=g++ -std=c++20 -g

driver: driver.cpp Record.h Operator.h FileReader.h Project.h Select.h Join.h FileWriter.h Attribute.h
	${CXX} driver.cpp -o driver

run: driver
	./driver

runv: driver
	valgrind --leak-check=full ./driver

clean:
	@echo "Cleaning..."
	rm -f driver
