from base cimport *
from enum import Enum

class PyRingType(Enum):
	PINK = 141
	YELLOW = 36
	RED = 1333
	BLUE = 84
	GREEN = 1022
	BLACK = 1330
	DASH_GREEN = 1704
	DASH_PINK = 1751
	CUSTOM = 1594

class PyPortalType(Enum):
	CUBE = 0
	SHIP = 0
	UFO = 0
	WAVE = 0
	ROBOT = 0
	SPIDER = 0
	GRAVITY_FLIPPED = 0
	GRAVITY_NORMAL = 0
	DIRECTION_REVERSE = 0
	DIRECTION_NORMAL = 0
	SIZE_NORMAL = 0
	SIZE_SMALL = 0
	DUAL_ON = 0
	DUAL_OFF = 0
	TELEPORT = 0

class PySpeedType(Enum):
	NORMAL = 0
	SLOW = 0
	FAST = 0
	FASTER = 0
	FASTEST = 0

class PyCollectibleType(Enum):
	KEY = 0
	HEART = 0
	FLASK = 0
	SKULL = 0
	COIN = 0