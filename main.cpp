#include <iostream>
#include <iomanip>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <string>
#include <chrono>
#include <cstring>
#include "mqtt/async_client.h"

//////////////////////////////////////////////////////////////////////////////////////////////

const std::string DFLT_SERVER_ADDRESS	{ "wss://localhost:8080" };
const std::string DFLT_CLIENT_ID		{ "cpp-mqtt" };
const std::string DFLT_USERNAME	    	{ "user" };
const std::string DFLT_PASSWORD 		{ "password" };

const std::string KEY_STORE				{ "tls.key" };
const std::string TRUST_STORE			{ "tls.crt" };

const std::string LWT_TOPIC				{ "last-will/cpp-mqtt" };
const std::string LWT_PAYLOAD			{ "Last will and testament from cpp-mqtt." };

const std::string TOPIC					{"my-topic"};

const int  	QOS 						= 2;
const auto 	TIMEOUT 					= std::chrono::seconds(10);
const int 	MY_SESSION_EXPIRY_INTERVAL 	= 60;	 			// in seconds
const int 	MY_MESSAGE_EXPIRY_INTERVAL 	= 20; 				// in seconds
const int 	MY_LWT_DELAY_INTERVAL 		= 10; 				// in seconds

#define MQTT5 // Comment to use MQTT version 3.1.1

/////////////////////////////////////////////////////////////////////////////////

class subscription_cb : public virtual mqtt::iaction_listener
{
	std::string name_;

	void on_failure(const mqtt::token& tok) override {
		auto t = std::time(nullptr);
    	auto tm = *std::localtime(&t);
    	std::cout 	<< "[" << std::put_time(&tm, "%H:%M:%S") << "] " 
					<< name_ << " failure ";
		auto top = tok.get_topics();
		if (top && !top->empty())
			std::cout 	<< "for topic: '"
						<< (*top)[0] << "', ..." << std::endl;
		std::cout << std::endl;
	}

	void on_success(const mqtt::token& tok) override {
		auto t = std::time(nullptr);
    	auto tm = *std::localtime(&t);
    	std::cout 	<< "[" << std::put_time(&tm, "%H:%M:%S") << "] " 
					<< name_ << " success ";
		auto top = tok.get_topics();
		if (top && !top->empty())
			std::cout 	<< "for topic: '"
						<< (*top)[0] << "', ..." << std::endl;
		std::cout << std::endl;
	}

public:
	subscription_cb(const std::string& name) : name_(name) {}
};

/**
 * A callback class for use with the main MQTT client.
 */
class mqtt_client_callback : 	public virtual mqtt::callback, 
								public virtual mqtt::iaction_listener
{
	// The MQTT client object to use in callback functions
	// will be passed via the constructor from main function
	mqtt::async_client& cli_;
	subscription_cb subCb_;
	
	// implement the derived pure virtual from action_listener
	void on_failure(const mqtt::token& tok) override {}

	// implement the derived pure virtual from action_listener
	void on_success(const mqtt::token& tok) override {}

	// Callback when connection is lost
	// (No reconnect function implemented in this sample)
	void connection_lost(const std::string& cause) override {
		auto t = std::time(nullptr);
    	auto tm = *std::localtime(&t);
    	std::cout 	<< "\n[" << std::put_time(&tm, "%H:%M:%S") << "] " 		
					<< "Connection lost" << std::endl;
		if (!cause.empty())
			std::cout << "\tcause: " << cause << std::endl;
		exit(1);
	}

	// Callback when meassage sent with QOS > 0
	void delivery_complete(mqtt::delivery_token_ptr tok) override {
		auto t = std::time(nullptr);
    	auto tm = *std::localtime(&t);
    	std::cout 	<< "[" << std::put_time(&tm, "%H:%M:%S") << "] " 
					<< "Received delivery acknowledgement (ACK) for mesage id: "
					<< (tok ? tok->get_message_id() : -1)
					<< " (only for QOS > 0)" << std::endl;
	}

	// Callback when a message arrives.
	void message_arrived(mqtt::const_message_ptr msg) override {
		auto t = std::time(nullptr);
    	auto tm = *std::localtime(&t);
    	std::cout 	<< "[" << std::put_time(&tm, "%H:%M:%S") << "] " 
					<< "Message arrived"
					<< "\n\ttopic: '" << msg->get_topic() << "'" 
					<< "\n\tpayload: '" << msg->to_string() << "'\n" << std::endl;
	}

	// Callback when the client connects
	// Publish a meassage 
	// Subscribe on a topic 
	void connected(const std::string& cause) override {
		auto t = std::time(nullptr);
    	auto tm = *std::localtime(&t);
    	std::cout 	<< "\n[" << std::put_time(&tm, "%H:%M:%S") << "] " 
					<< "Connection success" << std::endl;
		
		// Send a welcome message
		std::cout 	<< "\n[" << std::put_time(&tm, "%H:%M:%S") << "] " 
					<< "Sending 🖐  message with QOS=" << QOS << " on topic 'hello'... " 
					<< std::endl;
#ifdef MQTT5
		auto pubproperties = mqtt::properties();	
		pubproperties.add({
			mqtt::property::MESSAGE_EXPIRY_INTERVAL,MY_MESSAGE_EXPIRY_INTERVAL
			});
		auto msg = mqtt::message::create(
			"hello","Cedalo is awesome! (paho c++ client v5)",
			QOS, true, pubproperties
			);
#else	
		auto msg = mqtt::make_message(
			"hello", "Cedalo is awesome! (paho c++ client v3.1.1)",
			QOS, true);	
#endif
		mqtt::delivery_token_ptr tok = cli_.publish(msg);
		tok->wait_for(1000); // just slow down for demo
		// Check if sent succesful
		if (tok->get_return_code() == mqtt::SUCCESS){
			std::cout 	<< "...OK, sent message with id: "
						<< tok->get_message_id() << std::endl;
		} else {
			std::cout << "...NOK, sent message" << std::endl;
		}
		
		// Subscribe to topic
		mqtt::token_ptr subtok = cli_.subscribe(TOPIC, QOS,nullptr, subCb_);
		subtok->wait_for(1000); // just slow down for demo

		// Check if sent succesful
		std::cout 	<< "[" << std::put_time(&tm, "%H:%M:%S") << "] " ;
		if (subtok->get_return_code() == mqtt::SUCCESS){
			std::cout << "...OK, subscribed" << std::endl;
		} else {
			std::cout << "...NOK, subscribed" << std::endl;
		}
	}

public:
	// constructor
	mqtt_client_callback(mqtt::async_client& cli, mqtt::connect_options& connOpts)
		: cli_(cli), subCb_("Subscription") {}

};

/////////////////////////////////////////////////////////////////////////////

using namespace std;

int main(int argc, char* argv[])
{
	string	address  = (argc > 1) ? string(argv[1]) : DFLT_SERVER_ADDRESS,
			clientID = (argc > 2) ? string(argv[2]) : DFLT_CLIENT_ID,
			username  = (argc > 3) ? string(argv[3]) : DFLT_USERNAME,
			password  = (argc > 4) ? string(argv[4]) : DFLT_PASSWORD;


	// Check if cert files are in place
	{
		ifstream tstore(TRUST_STORE);
		if (!tstore) {
			cerr << "The trust store file does not exist: " << TRUST_STORE << endl;
			return 1;
		}

		ifstream kstore(KEY_STORE);
		if (!kstore) {
			cerr << "The key store file does not exist: " << KEY_STORE << endl;
			return 1;
		}
    }

	// Create the client object
	cout << "Initializing for server '" << address << "'..." << endl;
	cout << "\nPress Q<Enter> or Ctrl+C to quit\n" << endl;
	
#ifdef MQTT5	
	mqtt::async_client client(address, clientID,mqtt::create_options(MQTTVERSION_5));
#else
	mqtt::async_client client(address,clientID,mqtt::create_options(MQTTVERSION_3_1_1));
#endif

	// Build the connect options, including SSL and a LWT message.
	auto sslopts = mqtt::ssl_options_builder()
			.trust_store(TRUST_STORE)
			.enable_server_cert_auth(false) // will fail for self signed
			.error_handler([](const std::string& msg) {
				std::cerr << "SSL Error: " << msg << std::endl;
				})
			.finalize();
	
#ifdef MQTT5	
	// Prepare v5 specific options
	// --> see source code header: paho.mqtt.cpp/src/mqtt/properties.h for all options

	// last will properties
	auto willproperties = mqtt::properties();
	willproperties.add({
		mqtt::property::MESSAGE_EXPIRY_INTERVAL,MY_MESSAGE_EXPIRY_INTERVAL
		});
	willproperties.add({
		mqtt::property::WILL_DELAY_INTERVAL,MY_LWT_DELAY_INTERVAL
		});
	
	auto willmsg = mqtt::message(LWT_TOPIC, LWT_PAYLOAD, QOS, true, willproperties);

	// client properties
	auto properties = mqtt::properties();
	properties.add({
		mqtt::property::SESSION_EXPIRY_INTERVAL,MY_SESSION_EXPIRY_INTERVAL
		});

	auto connopts = mqtt::connect_options_builder()
		.properties(properties)		
		.user_name(username)
		.password(password)
		.mqtt_version(MQTTVERSION_5)
		.will(willmsg)
		.ssl(sslopts)
		.connect_timeout(TIMEOUT)
		.clean_start(true) // v5 specific
		.finalize();
#else
	// 
	auto willmsg = mqtt::message(LWT_TOPIC, LWT_PAYLOAD, QOS, true);

	// Case version < 5
	auto connopts = mqtt::connect_options_builder()
		.user_name(username)
		.password(password)
		.mqtt_version(MQTTVERSION_3_1_1)
		.will(std::move(willmsg))
		.ssl(std::move(sslopts))
		.connect_timeout(TIMEOUT)
		.clean_session(true)
		.finalize();
#endif

	// Create a callback object with the client and connection options
	mqtt_client_callback cb(client,connopts);

	// Set the callback object for the client
	client.set_callback(cb);						
	cout << "OK. Client initiaized and callback functions created" << endl;

	try {
		// Connect using SSL/TLS
		cout << "\nConnecting..." << endl;
		mqtt::token_ptr conntok = client.connect(connopts);
		cout << "Waiting for the connection..." << endl;

		conntok->wait_for(TIMEOUT);
		if (!client.is_connected()){
			cout << "...timeout..." << endl;
			return -1;
		}
		
		// Just block till user tells us to quit.
		while (std::tolower(std::cin.get()) != 'q');

		// Disconnect
		cout << "\nDisconnecting..." << endl;
		client.disconnect()->wait();
		cout << "...OK" << endl;
	}
	catch (const mqtt::exception& exc) {
		cerr << exc.what() << endl;
		return 1;
	}

 	return 0;
}
