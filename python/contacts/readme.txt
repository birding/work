
python-2.7.14.amd64.msi
windows����������
ϵͳ�������û�������Path�м� ;C:\Python27\;C:\Python27\Scripts;

C:\Users\Shanping>python --version
Python 2.7.14

C:\Users\Shanping>pip install virtualenv

cd F:\python\contacts

f:\python\contacts>virtualenv env

env\Scripts\activate

pip install vobject

(env) f:\python\contacts>pip freeze >requirements.txt
##pip install -r requirements.txt

(env) F:\python\contacts>python
>>> import vobject
>>> j = vobject.vCard()
>>> j.add('n')
<N{}    >
>>> j.n.value = vobject.vcard.Name( family='Harris', given='Jeffrey' )
>>> j.prettyPrint()
 VCARD
    N:  Jeffrey  Harris
>>>


s = """
BEGIN:VCARD
VERSION:3.0
EMAIL;TYPE=INTERNET:jeffrey@osafoundation.org
FN:Jeffrey Harris
N:Harris;Jeffrey;;;
END:VCARD
"""
>>> v = vobject.readOne( s )
>>> v.prettyPrint()


VCARD3.0
https://www.w3.org/TR/vcard-rdf/
RFC 2426 (vCard 3.0)


��ά����Ƭ�ĸ�ʽ - vcard
https://blog.csdn.net/sasoritattoo/article/details/17119123


Parses iCalendar and vCard files into Python data structures
https://pypi.org/project/vobject/

BEGIN:VCARD
VERSION:3.0
N:;��¹�;;;
FN:��¹�
TEL;TYPE=�ֻ�:18811102134
TEL;TYPE=�ֻ�:01050818443
TEL;TYPE=�ֻ�:19919898496
ORG:baidu
END:VCARD
