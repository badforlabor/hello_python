# 导入并立即执行这个文件！！
# 不过，不推荐这种写法，否则上下文，就乱了！

from . import func
from . import deco

print("import and run immediately!")

func.test_all()
deco.test_all()
