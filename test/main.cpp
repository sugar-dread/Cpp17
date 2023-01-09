#include <curl/curl.h>
#include <iostream>
#include <string>

#include <gumbo.h>

#include <rapidjson/document.h>
#include <rapidjson/error/en.h>
#include <rapidjson/filereadstream.h>
#include <rapidjson/rapidjson.h>
#include <rapidjson/schema.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

static size_t WriteCallback( void* contents, size_t size, size_t nmemb, void* userp ) {
  ( (std::string*)userp )->append( (char*)contents, size * nmemb );
  return size * nmemb;
}

std::string str_replace( std::string search, std::string replace, std::string& subject ) {
  size_t count;
  for ( std::string::size_type pos {}; subject.npos != ( pos = subject.find( search.data(), pos, search.length() ) ); pos += replace.length(), ++count ) {
    subject.replace( pos, search.length(), replace.data(), replace.length() );
  }

  return subject;
}

std::string extract_text( GumboNode* node ) {
  if ( node->type == GUMBO_NODE_TEXT ) {
    return std::string( node->v.text.text );
  } else if ( node->type == GUMBO_NODE_ELEMENT && node->v.element.tag != GUMBO_TAG_SCRIPT && node->v.element.tag != GUMBO_TAG_STYLE ) {
    std::string contents = "";
    GumboVector* children = &node->v.element.children;
    for ( unsigned int i = 0; i < children->length; ++i ) {
      std::string text = extract_text( (GumboNode*)children->data[i] );
      if ( i != 0 && !text.empty() ) {
        contents.append( "" );
      }

      contents.append( str_replace( ":", ">", text ) );
    }

    return contents;
  } else {
    return "";
  }
}

std::string find_definitions( GumboNode* node ) {
  std::string res = "";
  GumboAttribute* attr;
  if ( node->type != GUMBO_NODE_ELEMENT ) {
    return res;
  }

  if ( ( attr = gumbo_get_attribute( &node->v.element.attributes, "class" ) ) && strstr( attr->value, "dtText" ) != NULL ) {
    res += extract_text( node );
    res += "\n";
  }

  GumboVector* children = &node->v.element.children;
  for ( int i = 0; i < children->length; ++i ) {
    res += find_definitions( static_cast<GumboNode*>( children->data[i] ) );
  }

  return res;
}

std::string scrape( std::string markup ) {
  std::string res = "";
  GumboOutput* output = gumbo_parse_with_options( &kGumboDefaultOptions, markup.data(), markup.length() );

  res += find_definitions( output->root );

  gumbo_destroy_output( &kGumboDefaultOptions, output );

  return res;
}

int main( void ) {
  std::string ticker = "PLTR";
  // std::string endpoint = "https://finance.yahoo.com/quote/" + ticker + "?p=" + ticker;
  // std::string endpoint = "https://query1.finance.yahoo.com/v7/finance/quote?symbols=" + ticker;

  // std::string endpoint = "https://cppquiz.org/quiz/question/13";

  // std::string endpoint = "https://bigpara.hurriyet.com.tr/altin/ceyrek-altin-fiyati/";
  std::string endpoint = "https://www.merriam-webster.com/dictionary/esoteric";

  CURL* curl;
  std::string response;

  curl_global_init( CURL_GLOBAL_ALL );

  curl = curl_easy_init();
  if ( !curl ) {
    std::cerr << "failed to init curl";
  }

  curl_easy_setopt( curl, CURLOPT_URL, endpoint.c_str() );
  curl_easy_setopt( curl, CURLOPT_WRITEFUNCTION, WriteCallback );
  curl_easy_setopt( curl, CURLOPT_WRITEDATA, &response );

  CURLcode res = curl_easy_perform( curl );
  if ( res != CURLE_OK ) {
    std::cerr << "Error sending HTTP request: " << curl_easy_strerror( res ) << std::endl;
    curl_easy_cleanup( curl );
    curl_global_cleanup();

    exit( EXIT_FAILURE );
  }

  char* content_type;
  curl_easy_getinfo( curl, CURLINFO_CONTENT_TYPE, &content_type );

  std::cout << "aaa: " << std::string( content_type ) << "\n";
  curl_easy_cleanup( curl );
  curl_global_cleanup();

  // std::cout << scrape( res ) << std::endl;

  std::cout << response << "\n";
  std::cout << scrape( response ) << std::endl;

  /*
    std::string html_string = R"(
    <html>
      <head>
        <title>Example Page</title>
      </head>
      <body>
        <h1>Welcome to my page!</h1>
        <p>This is an example HTML page.</p>
        <ul>
          <li>Item 1</li>
          <li>Item 2</li>
          <li>Item 3</li>
        </ul>
      </body>
    </html>
    )";

    std::string xml_string = R"(
    <root>
      <element1>Hello</element1>
      <element2>World</element2>
    </root>
    )";

    // Use Boost Property Tree to parse the HTML string into a property tree
    // boost::property_tree::ptree pt;
    // std::istringstream html_stream( html_string1 );
    // boost::property_tree::read_xml( html_stream, pt );

    // Create a JSON object to hold the parsed data
    rapidjson::Document json_doc;
    json_doc.SetObject();
  */
  // Iterate through the nodes of the property tree and add the necessary information to the JSON object

  /*
  for ( auto& child : pt ) {
    std::cout << child.first << "asdsad\n";
    if ( child.first == "html.head.title" ) {
      // Add the contents of the <title> tag to the JSON object
      json_doc.AddMember( "title", rapidjson::GenericStringRef( child.second.data().c_str() ), json_doc.GetAllocator() );
    } else if ( child.first == "html.body.h1" ) {
      // Add the contents of the <h1> tag to the JSON object

      json_doc.AddMember( "h1", rapidjson::GenericStringRef( child.second.data().c_str() ), json_doc.GetAllocator() );
    } else if ( child.first == "html.body.p" ) {
      // Add the contents of the <p> tag to the JSON object

      json_doc.AddMember( "p", rapidjson::GenericStringRef( child.second.data().c_str() ), json_doc.GetAllocator() );
    }
  }


  // Print the JSON object to the console
  rapidjson::StringBuffer buffer;
  rapidjson::Writer<rapidjson::StringBuffer> writer( buffer );
  json_doc.Accept( writer );
  std::cout << buffer.GetString() << std::endl;

  rapidjson::Document doc;
  doc.SetObject();
  */
  /*
  if ( xml_string.empty() ) {
    std::cerr << "No cfg file!\n";
    exit( EXIT_FAILURE );
  }

  if ( doc.Parse( xml_string.c_str() ).HasParseError() ) {
    std::cerr << "cfg file is not a valid JSON!\n";
    exit( EXIT_FAILURE );
  }

    // Access the stock price data
    double price = doc["quoteResponse"]["result"][0]["regularMarketPrice"].GetDouble();
    std::string companyName = doc["quoteResponse"]["result"][0]["fullExchangeName"].GetString();

    std::cout << companyName << ": " << price << std::endl;
  */
  return 0;
}
