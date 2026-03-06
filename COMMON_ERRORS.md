# MQuant 常见报错总结

**生成代码前必读 - 对照检查清单**

> 💡 **提示**：这是 Skill 文档。如果你想添加个人踩过的坑而不被更新覆盖，请创建 COMMON_ERRORS.user.md 文件。
> 
> 详见 README.md

---

---

## ❌ 错误1：结构体字段不存在

**报错：** `'Tick' object has no attribute 'last'`

**原因：** 使用了文档中不存在的字段

**解决：**
- ✅ `tick.current` (最新价)
- ❌ `tick.last` (不存在)
- ✅ `pos.symbol` (持仓代码)
- ❌ `pos.code` (不存在)

---

## ❌ 错误2：API返回类型错误

**报错：** `'str' object has no attribute 'symbol'`

**原因：** `get_positions()` 返回字典 `dict<symbol, Position>`，不是列表

**错误：**
```python
for pos in positions:  # 遍历得到的是symbol字符串
    pos.symbol  # 报错！
```

**正确：**
```python
pos = positions.get(g_security)
amount = pos.amount if pos else 0
```

---

## ❌ 错误3：subscribe参数错误

**报错：** 没有行情数据 / 策略不运行

**原因：** 使用了不支持的频率参数

**错误：** `subscribe(symbol, '1d')`

**正确：** `subscribe(symbol, MarketDataType.KLINE_1M)`

---

## ❌ 错误4：文件名超过11字符

**报错：** Matic无法加载策略

**原因：** Python文件名限制11字符（不含扩展名）

**错误：** `ultra_grid_v1.py` (13字符)

**正确：** `ugrid_v1.py` (9字符)

---

## ❌ 错误5：缺少必要导入

**报错：** `name 'log' is not defined`

**原因：** 没有导入mquant_api

**正确：**
```python
from mquant_api import *
from mquant_struct import *
```

---

## ❌ 错误6：回调函数参数不匹配

**报错：** `handle_tick() takes 3 positional arguments but 4 were given`

**原因：** 回调函数参数与文档不符

**正确：**
```python
def handle_tick(context, tick, msg_type):  # 3个参数
def handle_data(context, kline_data):      # 2个参数
```

---

## ❌ 错误7：全局变量未声明global

**报错：** `UnboundLocalError: local variable referenced before assignment`

**原因：** 修改全局变量没有声明 `global`

**正确：**
```python
def handle_tick(context, tick, msg_type):
    global g_last_idx  # 必须声明
    g_last_idx = 0
```

---

## ✅ 生成代码前自检清单

- [ ] 结构体字段核对 `mquant_struct.py`
- [ ] `get_positions()` 按字典访问
- [ ] `subscribe()` 使用 `MarketDataType`
- [ ] 文件名 ≤ 11 字符
- [ ] 已导入 `mquant_api` 和 `mquant_struct`
- [ ] 回调函数参数与文档一致
- [ ] 全局变量声明 `global`
- [ ] 日志记录完整

---

**完整文档：** `reference/mquant_inside_python_document/`
