import sys
 
from pyspark import SparkContext, SparkConf

if __name__ == "__main__":
	
	sc = SparkContext("local","PySpark Inverted Index")
	
	rdd = sc.wholeTextFiles(sys.argv[1])
	rdd1 = rdd.flatMap(lambda x: [(x[0],line) for line in x[1].lower().split("\n")])  
	rdd2 = rdd1.flatMap(lambda x:[((x[0].split("/")[-1]),word) for word in x[1].split(" ")])
	rdd3 = rdd2.filter(lambda x: len(x[1])> 0)
	rdd4 = rdd3.map(lambda x: (x[1],[x[0]]))
	output = rdd4.reduceByKey(lambda a,b: a+b)
	
	output.saveAsTextFile(sys.argv[2])
	
	
	
