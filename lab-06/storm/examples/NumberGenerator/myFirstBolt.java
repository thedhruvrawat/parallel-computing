package sExample1;

import org.apache.storm.topology.BasicOutputCollector;
import org.apache.storm.topology.OutputFieldsDeclarer;
import org.apache.storm.topology.base.BaseBasicBolt;
import org.apache.storm.tuple.Fields;
import org.apache.storm.tuple.Tuple;
import org.apache.storm.tuple.Values;

public class myFirstBolt extends BaseBasicBolt{
	public void execute(Tuple input, BasicOutputCollector collector) {
		Integer number = input.getInteger(0);
		number *= 2;
		collector.emit(new Values(number));
		System.out.println(number);
	}
	
	public void declareOutputFields(OutputFieldsDeclarer declarer) {
		declarer.declare(new Fields("field"));
	}
	
	public void cleanup() {}
}
