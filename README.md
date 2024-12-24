# Query Execution Engine

This project is a search query execution engine. Think of it as how an SQL query actually finds the records in a database. Unlike SQL, this implementation is rooted in relational algebra. The available operations in this execution engine are Project, Select, and Join. This implementation allows for arbitrary queries, i.e. records in the data set are arbitrary and the length of the query may be completely arbitrary. Whatever you can execute with the three available operators can be completed with this engine. The limitations with the format of data.

In the data files, the first line must be the names of each attribute in order. The next line must be the type of the data for which the attribute will hold. Data types have been limited to "Integer", "Double", and "String" which are case sensitive. The next line is just junk and will be ignored. From then on shall be the data. Each Attribute must be comma delimited. 

# Usage
