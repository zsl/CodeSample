#coding:utf8

import urllib2
import re

debug = 0
def _print(s):
    if debug:
        print s

prog_charset = re.compile(r'<meta\s+.*?content=".*?charset=([\w\d-]+).*?".*?>')
def get_charset(html):
    charsetObj = prog_charset.search(html)
    if charsetObj:
        return charsetObj.group(1)

forum = 'http://192.168.9.100/discuz/forum.php'
fnet = urllib2.urlopen(forum)
forum_html = fnet.read()
_print(fnet.geturl() )
# info = fnet.info()
# for key in info:
#     print key,':', info[key]

# 获取 charset
charset = get_charset(forum_html)
if charset:
    _print(charset)

# 获取suggest url
prog_suggest = re.compile(r'<a\s+href\s*=\s*"(.*?)">\s*产品建议\s*</a>')
suggest_url_obj = prog_suggest.search(forum_html)

suggest_url = None
if suggest_url_obj :
    suggest_url = suggest_url_obj.group(1)
    _print(suggest_url)

get_data = suggest_url.split('?')
if len(get_data) > 1:
    get_data = get_data[1]
else:
    get_data = get_data[0]

# 打开suggest
fnet = urllib2.urlopen(forum+'?%s' % get_data)
suggest_html = fnet.read()
_print(fnet.geturl() )
charset = get_charset(suggest_html)
_print(charset)

# 获取最新的 产品建议
prog_dest = re.compile(r'<th.*?>\s*<a\s+href="(.*?)".*?>(.*?产品建议)</a>')
match_result = prog_dest.search(suggest_html)
dsturl = None
title = None
if match_result:
    dsturl = match_result.group(1)
    dsturl = dsturl.replace('&amp;', '&')
    title  = match_result.group(2)
    _print('href:' + match_result.group(1) )
    _print('title:' + title if charset is None else title.decode(charset) )

get_data = dsturl.split('?')
if len(get_data) > 1:
    get_data = get_data[1]
else:
    get_data = get_data[0]

# 获取提交产品建议的人的名字
fnet = urllib2.urlopen(forum+'?%s' % get_data)
print 'get url: %s' % fnet.geturl()
html = fnet.read()
charset = get_charset(html)
_print(charset)

prog = re.compile(r'<div class="authi">\s*<a.*?>(.*?)</a>')

# for author in prog.finditer(html):
#     a =  author.group(1)
#     print a if not charset else a.decode(charset)

good_ones = [author.lower() if not charset else author.group(1).decode(charset).lower() for author in prog.finditer(html)]

import ConfigParser

config = ConfigParser.ConfigParser()
config.readfp(open('wholost.ini', 'r'))
members = config.items('member')

print 'bad boys:'
for v_name, t_name in members:
    if v_name.decode('gbk') not in good_ones:
        print t_name
