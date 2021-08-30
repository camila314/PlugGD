import json, base64, hashlib, random, os
requests = os
from itertools import cycle
izip = zip
import gzip
try:
	from urllib.parse import urlencode
	import urllib.request as urllib2
except:
	from urllib import urlencode
	import urllib2
from math import sqrt
def makeSeed(encoded):
	if len(encoded)< 49:
		seed2 = encoded
	else:
		seed2=""
		i=0
		while i<50:
			seed2+= encoded[(len(encoded)//50)*i]
			i+=1
	return xor(hashlib.sha1(seed2.encode()+b"xI25fpAapCQg").hexdigest(),"41274")
def urlopen(url,p):
	t = urllib2.Request(url,p.encode())
	return urllib2.urlopen(t)
starting = {"gameVersion":"21","binaryVersion":"35","gdw":"0"}
def getAccountID(username):
	diction = {"str":username,"total":"0","page":"0","secret":"Wmfd2893gb7"}
	d = dict(diction)
	d.update(starting)
	
	r = requests.post("http://www.boomlings.com/database/getGJUsers20.php",data=d)
	
	try: return r.text.split(":")[21]
	except: raise ValueError("Incorrect Username")
def xor(data, key):
	xored = ''.join(chr(ord(x) ^ ord(y)) for (x,y) in zip(data, cycle(key)))
	return base64.encodestring(xored.encode()).strip()
class Block:
	items = {"1_9_color":"19","green":"8","player_color_1":"15","player_color_2":"16","text":"31","rotation_times_360":"69","target_group_id":"51","z_layer":"24","instant_count_comparison":"88","copy_opacity":"60","custom_rotation_speed":"97","substract_count":"78","pulse_target_type":"52","move_target_enabled":"100","count":"77","spawn_delay":"63","color1":"21","pulse_exclusive":"86","color2":"22","rotation":"6","group_ids":"57","follow_player_y_offset":"92","move_trigger_9_units":"29","multi_trigger":"87","rotation_lock":"70","red":"7","group_parent":"34","dont_enter":"67","touch_dual_mode":"89","spawn_triggered":"62","is_flipped_horizontal":"4","duration":"10","blue":"9","activate_group":"56","dynamic_block":"93","follow_player_y_delay":"91","move_lock_to_player_y":"59","move_lock_to_player_x":"58","target_color_id":"23","follow_y_mod":"73","follow_player_y_speed":"90","is_flipped_vertical":"5","touch_toggle":"82","item_id":"80","glow_disabled":"96","copied_color_hsv_values":"49","hold_mode":"81","pickup_mode":"79","editor_layer_2":"61","editor_layer_1":"20","pulse_detail_only":"66","hsv_enabled_1":"41","hsv_enabled_2":"42","shake_strength":"75","is_active_trigger_type":"36","move_target_pos_coordinates":"101","portal_checked":"13","teleportation_yellow_distance":"54","blending":"17","pulse_hold":"46","pulsing_animation_randomize":"106","size":"32","disable_rotation":"98","collision_block_b_id":"95","high_detail":"103","rotation_degrees":"68","pulse_main_only":"65","pulse_fade_out":"47","x_position":"2","z_order":"25","dont_fade":"64","easing_rate":"85","hsv_values_1":"43","hsv_values_2":"44","follow_x_mod":"72","pulse_fade_in":"45","spawn_editor_disable":"102","move_trigger_x_units":"28","blockid":"1","linked_group":"108","follow_player_y_max_speed":"105","touch_triggered":"11","opacity":"35","copied_color_id":"50","pulse_mode":"48","y_position":"3","shake_interval":"84","pulsing_animation_speed":"107","count_multi_activate":"99","easing":"30","follow_group_id":"71"}
	otheritems = {v: k for k, v in items.items()}
	def __init__(self,**kwargs):
		self.levelstring = ""
		self.json = {"blockid":str(kwargs.get("blockid")),"x_position":str(kwargs.get("x_position")),"y_position":str(kwargs.get("y_position"))}
		for i in kwargs: self.json[i] = kwargs[i]
	def __getitem__(self,e):
		if e in self.json:
			return self.json[e]
		else:
			return '0'
	def __setitem__(self,e,f):
		self.json[e] = f
	def __str__(self):
		ar = []
		for i in self.json:
			ar.append(str(Block.items[i]))
			ar.append(str(self.json[i]))
		return ','.join(ar)
	def change(self,**kwargs):
		a = Block(**self.json)
		for i in kwargs:
			a[i] = kwargs[i]
		return a
	@staticmethod
	def blockFromString(stri):
		a = stri.split(',')
		evens = a[1::2]
		odds = a[0::2]
		jso = {}
		for x in range(len(odds)):
			i = odds[x]
			if i in Block.otheritems:
				jso[Block.otheritems[i]] = evens[x]
		bloc = Block(**jso)
		return bloc
	def __repr__(self):
		return self.__str__()
	def coords(self):
		return [float(self['x_position']), float(self['y_position'])]
	def setCoords(self, x='d', y='d'):
		if x!='d':
			self['x_position'] = str(x)
		if y!='d':
			self['y_position'] = str(y)
	def rotate(self, degrees=0.):
		rot = float(self['rotation'])
		self['rotation'] = str(rot+degrees)
class Header:
	key = {'mini': 'kA3', 'ground_line': 'kA17', 'background': 'kA6', 'dual': 'kA8', 'fade_out': 'kA16', 'ground': 'kA7', 'song_offset': 'kA13', 'colors': 'kS38', '2_player': 'kA10', 'vehicle': 'kA2', 'font': 'kA18', 'speed': 'kA4', 'fade_in': 'kA15',"nothing":"kA14","idk":"kA9","nutin":"kA11","ww":"kS39"}
	def __init__(self,**kwargs):
		self.json = {'colors': '1_40_2_125_3_255_11_255_12_255_13_255_4_-1_6_1000_7_1_15_1_18_0_8_1|1_0_2_102_3_255_11_255_12_255_13_255_4_-1_6_1001_7_1_15_1_18_0_8_1|1_0_2_102_3_255_11_255_12_255_13_255_4_-1_6_1009_7_1_15_1_18_0_8_1|1_255_2_255_3_255_11_255_12_255_13_255_4_-1_6_1002_5_1_7_1_15_1_18_0_8_1|1_0_2_255_3_125_11_255_12_255_13_255_4_-1_6_1005_5_1_7_1_15_1_18_0_8_1|1_0_2_255_3_125_11_255_12_255_13_255_4_-1_6_1006_5_1_7_1_15_1_18_0_8_1|1_0_2_0_3_0_11_255_12_255_13_255_4_-1_6_3_7_1_15_1_18_0_8_1|','song_offset': '0','fade_in': '0','fade_out': '0',"nothing":"",'background': '1', 'ground': '0', 'ground_line': '0', 'font': '0',"ww":"0","vehicle":'0','mini':'0','dual':'0',"speed":"0","idk":"0","2_player":"0","nutin":"0"}
		for i in kwargs:
			self.json[i] = kwargs[i]
	def __getitem__(self,e):
		return self.json[e]
	def __setitem__(self,e,f):
		self.json[e] = str(f)
	def __str__(self):
		ar = []
		for i in self.json:
			ar.append(str(Header.key[i]))
			ar.append(str(self.json[i]))
		return ','.join(ar)
	def __repr__(self):
		return self.__str__()
class Level:
	@staticmethod
	def downloadLevel(levelname,lid):
		return levelString(levelname,gzip.decompress(base64.b64decode(requests.post('http://www.boomlings.com/database/downloadGJLevel22.php','gameVersion=21&binaryVersion=35&gdw=0&levelID=%s&inc=0&extras=0&secret=Wmfd2893gb7'%lid,headers={'Content-Type':'application/x-www-form-urlencoded'}).text.split(':')[7],'-_')).decode())
	def getfGroupID(self):
		ids = [0]
		for i in self.blocks:
			try:
				ids.append(int(i.json["group_ids"]))
			except Exception as e: print(e)
		ids.sort()
		for i in range(1,len(ids)):
			if ids[i] != ids[i-1]+1:
				return i
	def blocksInGroup(self, groupid):
		out = []
		for b in self.blocks:
			gs = str(b['group_ids']).split(',')
			if str(groupid) in gs:
				out.append(b)
		return out
	def __init__(self,levelname,**kwargs):
		self.header = Header(**kwargs)
		self.blocks = []
		self.name = levelname
		self.str = ""
	def addBlock(self,a,b,c,**kwargs):
		aa = {"x_position":b,"y_position":c,"blockid":a}
		b = Block(**dict(aa,**kwargs))
		self.blocks.append(b)
		return b
	def addBlocks(self, *args):
		for i in args:
			if hasattr(i,'coords'):
				self.blocks.append(i)
	def uploadLevel(self,username,password,description="",stars=0,unlisted=0,lpassword=0,songid=0,audiotrack=1,twoPlayer=0,fromString=False,coins='0',url="http://boomlings.com/database/uploadGJLevel21.php",version=1,levelid=0):
		if not fromString:
			self.encodee()
		else:
			self.encoded = self.str
		seed2 = makeSeed(self.encoded)
		dictionary = {"accountID":getAccountID(username),"gjp":xor(password,"37526"),"userName":username,"levelID":levelid,"levelName":self.name,"levelDesc":base64.b64encode(description.encode()),"levelVersion":str(version),"levelLength":"0","audioTrack":str(audiotrack),"auto":"0","password":str(lpassword),"original":"0","twoPlayer":str(twoPlayer),"songID":str(songid),"objects":str(len(self.blocks)),"coins":coins,"requestedStars":str(stars),"unlisted":str(unlisted),"wt":"13","wt2":"0","ldm":"0","seed":"BBUEIHF1rd","extraString":"0_0_0_0_0_0_0_0_0_0_0_0_0_0_0_0_0_0_0_0_0_0_0_0_0_0_0_0_0_0_0_0_0_0_0_0_0_0_0_0_0_0_0_0_0_0_0_0_0_0_0_0_0_0_0","seed2":seed2,"levelString":self.encoded,"secret":"Wmfd2893gb7","levelInfo":"H4sIAAAAAAAAEzOwNrQ20DMwsrY21DMztbYGAJp7I8gQAAAA"}
		dictionary.update(starting)
		return urlopen(url,urlencode(dictionary).replace('%3D','=')).read().decode()
	def __str__(self):
		return str(self.header) + ';' + ';'.join([str(x) for x in self.blocks])
	def encodee(self):
		data = base64.b64encode((gzip.compress(self.__str__().encode(),compresslevel=6)))
		self.encoded = data.replace(data[4:13],b"AAAAAAAAE").replace(b"+",b"-").replace(b"/",b"_").decode()
def getSong(lid):
		return requests.post('http://www.boomlings.com/database/downloadGJLevel22.php','gameVersion=21&binaryVersion=35&gdw=0&levelID=%s&inc=0&extras=0&secret=Wmfd2893gb7'%lid,headers={'Content-Type':'application/x-www-form-urlencoded'}).text.split(':')[49]
def levelString(levelname,lstr):
	a = Level(levelname)
	lst = lstr.split(";")
	a.header = lst[0]
	for i in lst[1:-1]:
		a.blocks.append(Block.blockFromString(i))
	return a
def downloadLevel(levelname,lid):
	base64.b64decode(requests.post('http://www.boomlings.com/database/downloadGJLevel22.php','gameVersion=21&binaryVersion=35&gdw=0&levelID=%s&inc=0&extras=0&secret=Wmfd2893gb7'%lid,headers={'Content-Type':'application/x-www-form-urlencoded'}).text.split(':')[7],'-_')
	return levelString(levelname,gzip.decompress(base64.b64decode(requests.post('http://www.boomlings.com/database/downloadGJLevel22.php','gameVersion=21&binaryVersion=35&gdw=0&levelID=%s&inc=0&extras=0&secret=Wmfd2893gb7'%lid,headers={'Content-Type':'application/x-www-form-urlencoded'}).text.split(':')[7],'-_')).decode())