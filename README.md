# Query Execution Engine

This project is a search query execution engine. Think of it as how an SQL query actually finds the records in a database. Unlike SQL, this implementation is rooted in relational algebra. The available operations in this execution engine are Project, Select, and Join. This implementation allows for arbitrary queries, i.e. records in the data set are arbitrary and the length of the query may be completely arbitrary. Whatever you can execute with the three available operators can be completed with this engine. The limitations with the format of data.

In the data files, the first line must be the names of each attribute in order. The next line must be the type of the data for which the attribute will hold. Data types have been limited to "Integer", "Double", and "String" which are case sensitive. The next line is just junk and will be ignored. From then on shall be the data. Each Attribute must be comma delimited. 

# Usage
In this implementation, every operator has three main member functions defined in the super class: open, close, and next. First, the open method initializes any operators being called and must be called before any work can actually be done. Similarly, the close method must be called after the work you want to do is done. The next method obtains the next page's worth of data.

Due to the operators allowing for arbitrarily complex queries, you can tie mulitple operators together. Thus you need only call open, close, and next on the highest order operator. This would then call each respective method all the way to the bottom. Here is how you can initialize the operators directly from a file:
```
Project op1("student.data", {"LastName", "GPA", "FirstName"});
```

Then to tie another operator, you can:
```
Project op2(&op1, {"FirstName", "GPA"});
```

For the Select operator:
```
Select op3("student.data", "GPA", ">", "3.0");
Select op4(&op2, "GPA", ">", "3.0");
```

For the Join operator:
```
Join op5("student.data", "advisor.data", "ID", "s_ID");
```
The Join operator also allows for the left operator to be another operator, but only allows the right operator to be an operator rather than a source file if the left operator is another operator already. This is more efficient due to how the join algorithm works (I do not worry about swapping each side to make it more efficient depending on the situations). Another quirk of the Join operator is that it does strictly equi-joins. No other type of join is supported.

Once you have all your operators initialized, here is a usage example:
```
  op2.open();

  int recordCount=0;
  int pageCount=0;

  vector<Record> page;
  do {
    pageCount++;
    page = op2.next();
    cout << "-----------------Page-----------------" << endl;
    for(auto record : page) {
      recordCount++;
      cout << record << endl;
    }
  } while (page.size() > 0);
  op2.close();
```
This example opens op2, whatever it was defined to be, and opens every subsequent operator constituting op2. It then creates a variable to count pages and records if you find that useful. Then it gets pages from the operator until the page becomes empty (there is no more data to be read). It also iterates through each page to print out the records. By overloading the print operator, printing records and attributes is as simple as utilizes the normal way to print in c++. Lastly, ensure you close the operator.
