package sExample1;

import java.util.Map;

import org.apache.storm.spout.SpoutOutputCollector;
import org.apache.storm.task.TopologyContext;
import org.apache.storm.topology.OutputFieldsDeclarer;
import org.apache.storm.topology.base.BaseRichSpout;
import org.apache.storm.tuple.Values;
import org.apache.storm.tuple.Fields;

public class myFirstSpout extends BaseRichSpout{
	private SpoutOutputCollector collector;
	
	private Integer i = 0;
	public void open(Map conf, TopologyContext context, SpoutOutputCollector collector) {
		this.collector  = collector;
	}
	public void nextTuple() {
		if(this.i < 100){
		this.collector.emit(new Values(this.i));
		this.i = this.i+1;;
	}
	}
	
	public void declareOutputFields(OutputFieldsDeclarer declarer) {
		declarer.declare(new Fields("field"));
	}
	
	public void ack(Object msgId) {}
	
	public void fail(Object msgId) {}
}
