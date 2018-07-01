import xml.dom.minidom

ELEMENT_NODE = xml.dom.Node.ELEMENT_NODE
def is_attribute_match(el, attribute_name_value):
    if not attribute_name_value:
        return True
    name, value = attribute_name_value
    attrs = el.attributes
    #print attribute_name_value, zip(attrs.keys(), attrs.values())
    if not attrs.has_key(name):
        return False
    if not value:
        return  True
    else:
        return attrs[name].value == value


def is_element_matching(el, name, attribute_name_value):
    return (el.nodeType == ELEMENT_NODE 
        and (name =='*' or (el.tagName == name))
        and is_attribute_match(el, attribute_name_value))


def elements_without_subdirs(xml_element, name, attribute_name_value = None):
    return [n for n in xml_element.childNodes if is_element_matching(n, name, attribute_name_value)] 


def elements(xml_element, name, attribute_name_value = None):
    parent, last_name = element_parent(xml_element, name)
    return elements_without_subdirs(parent, last_name, attribute_name_value)    


def element_parent(xml_element, name):
    i_dot = name.find("/")
    if (i_dot == -1):
        return (xml_element, name)
    else:
        return element_parent(
            element_without_subdirs(xml_element, name[:i_dot], None),
            name[i_dot+1:])


def element_without_subdirs(xml_element, name, attribute_name_value = None):
    elem_list = elements_without_subdirs(xml_element, name, attribute_name_value);
    assert len(elem_list) > 0, ("can't find xml-element '%s'"%name)
    assert len(elem_list) == 1, ("xml-element '%s' is duplicated"%name)
    return elem_list[0]


def element(xml_element, name, attribute_name_value = None):
    parent, last_name = element_parent(xml_element, name)
    return element_without_subdirs(parent, last_name, attribute_name_value)


def text(element):
    rc = ""
    for node in element.childNodes:
        if node.nodeType == node.TEXT_NODE:
            rc = rc + node.data
    return rc


def parse_file(filename):
    try:
        return xml.dom.minidom.parse(filename)
    except Exception, e:
        raise Exception, "can't parse xml-file '%s' (%s)" % (filename, e)


def attr(element, attr_name):
    return text(element.attributes[attr_name])


def has_attr(element, attr_name):
    return element.attributes.has_key(attr_name)


def try_attr(element, attr_name):
    a = element.attributes.get(attr_name)
    if a:
        return text(a)
    else:
        return None
