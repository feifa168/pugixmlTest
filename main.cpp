#include "pugixml.hpp"
#include <iostream>
#include <cstring>

using std::cout;
using std::endl;
using namespace pugi;

// <encrypt>
//     <src>IdsConfig.jar</src>
//     <dst>IdsConfigEnc.jar</dst>
//     <files [loadall="true"]>
//         <file type="package">com.ids.config</file>
//         <file type="package">com.ids.copy</file>
//         <file type="package">com.ids.param</file>
//         <file type="package">com.ids.shell</file>
//         <file type="package">com.ids.syslog</file>
//         <file type="package">com.ids</file>
//     </files>
// </encrypt>
void displayNodeNameAndValue(pugi::xml_node &node) {
    //cout << "node name is " << node.name() << " value is " << node.value() << " chile_value is " << node.child_value() << endl;
    cout << "node name is " << node.name() << " value is " << node.value() << " text is " << node.text().get() << endl;
}

void setNodeTextAndAttr(xml_node &node, const char*text, const char*attr) {
    node.text().set(text);
    node.append_attribute("type").set_value(attr);
}

void testWriteXml(const char* xml) {
    xml_document doc;
    xml_node ndRoot = doc.append_child("encrypt");
    xml_node ndCur = ndRoot.append_child("dst");
    ndCur.text().set("src.jar");
    ndRoot.insert_child_before("src", ndCur).text().set("src.jar");

    xml_node ndFiles = ndRoot.insert_child_after("files", ndCur);
    ndFiles.append_attribute("loadall").set_value("true");
    const char texts[][32]= {
        "com.ids.config",
        "com.ids.copy",
        "com.ids.param",
        "com.ids.shell",
        "com.ids.syslog",
        "com.ids"
    };

    int len = sizeof(texts)/sizeof(texts[0]);
    for (int i=0; i<len; i++) {
        ndCur = ndFiles.append_child("file");
        setNodeTextAndAttr(ndCur, texts[i], "package");
    }

    pugi::xml_node decl = doc.prepend_child(pugi::node_declaration);
    decl.append_attribute("version") = "1.0";
    decl.append_attribute("encoding") = "utf-8";
    doc.save(cout);
    doc.save_file(xml, "  "/*, format_default, encoding_utf8*/);
}

void testReadXml(const char *xml) {
    xml_document doc;
    xml_parse_result result = doc.load_file(xml, parse_default, encoding_auto);
    if (status_ok != result.status) {
        cout << "load file fail, return is " << result.status << endl;
        return;
    }

    xml_node ndEnc = doc.child("encrypt");
    xml_object_range<xml_node_iterator> iters = ndEnc.children();
    //for (xml_node_iterator it=iters.begin(); it!=iters.end(); it++) {
    for (xml_node_iterator it : iters) {
        xml_node &ndCur = *it;
        displayNodeNameAndValue(ndCur);
        if (0 == strcmp(ndCur.name(), "files")) {
            xpath_node_set xpaths = ndCur.select_nodes("file");
            for (xpath_node node : xpaths) {
                xml_node ndFile = node.node();
                displayNodeNameAndValue(ndFile);
            }
        }
    }
}

void testFromString() {
    pugi::xml_document doc;

    // get a test document
    doc.load_string("<project><name>test</name><version>1.1</version><public>yes</public></project>");

    pugi::xml_node project = doc.child("project");

    // tag::access[]
    std::cout << "Project name: " << project.child("name").text().get() << std::endl;
    std::cout << "Project version: " << project.child("version").text().as_double() << std::endl;
    std::cout << "Project visibility: " << (project.child("public").text().as_bool(/* def= */ true) ? "public" : "private") << std::endl;
    std::cout << "Project description: " << project.child("description").text().get() << std::endl;
    // end::access[]

    std::cout << std::endl;

    // tag::modify[]
    // change project version
    project.child("version").text() = 1.2;

    // add description element and set the contents
    // note that we do not have to explicitly add the node_pcdata child
    project.append_child("description").text().set("a test project");
    // end::modify[]

    doc.save(std::cout, "\t");
}
void testEncConfig() {
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file("enc_config.xml", pugi::parse_default, pugi::encoding_auto);
    if (!result) {
        cout << "load_file(enc_config.xml fail" << endl;
        return;
    }

    pugi::xml_node ndSrc = doc.child("encrypt").child("src");
    displayNodeNameAndValue(ndSrc);
    pugi::xml_node ndDst = doc.child("encrypt").child("dst");
    displayNodeNameAndValue( ndDst );

    bool isAll = doc.select_node("/encrypt/files").node().attribute("loadall").as_bool();
    cout << "=========================" << endl;
    for (pugi::xml_node tool: doc.child("encrypt").child("files").children("file")) {
        displayNodeNameAndValue(tool);
        pugi::xml_attribute attr = tool.attribute("type");
        cout << "    attr(type) name " << attr.name() <<  " value is " << attr.value() << endl;
    }

    cout << "=========================" << endl;
    pugi::xpath_node_set nodes = doc.select_nodes("/encrypt/files/file[@type=\"package\"]");
    for (pugi::xpath_node node: nodes) {
        pugi::xml_node tool = node.node();
        displayNodeNameAndValue(tool);
        pugi::xml_attribute attr = tool.attribute("type");
        cout << "    attr(type) name " << attr.name() <<  " value is " << attr.value() << endl;
    }
}

int main() {
    testFromString();

    const char *xml = "test.xml";
    testWriteXml(xml);
    testReadXml(xml);

    testEncConfig();

    return 0;
}