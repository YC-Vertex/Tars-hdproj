# Tars

可爱的小机器人+智能家居管理终端，创意源自Intersteller中的机器人Tars，2019夏季THUEE电子设计大赛作品

## Tars Serial Protocol

每条指令包(pkg, package)不得超过128个字符，以`$`作为起始符，`<`作为终止符。整条指令分为指令头(hdr, header)和指令消息(msg, message)两部分，中间用`:`隔开。

指令示例：`$ACONN1:27;<`

### 指令头

指令头分为三部分：设备代码(dvc, device)、指令类型（cmd, command)、指令消息条数。

设备代码占两个字符位，`TS`代表机器人本身，相关执行代码在`tSelf.h/.cpp`中；`AC`代表空调，相关执行代码在`tAirConditioner.h/.cpp`中；`LT`代表灯光，相关执行代码在`tLight.h/.cpp`中；`PW`代表电源，相关执行代码在`tPower.h/.cpp`中；其余指令的设备代码为`OT`（暂无）。

指令类型占三个字符位，详见指令类型清单。

指令消息条数占一个字符为，可选值为`0~9`，必须与指令消息中`;`的数目保持一致（详见指令消息）。

### 指令消息

指令消息中包含nmsg个（nmsg为指令消息条数）字段(sct, section)，每个字段以`;`结尾。若字段中包含多个值(val, value)，则每个值之间用`,`隔开，详见指令类型清单。

### 指令类型清单

#### TS - Tars

- [ ] `SLP`：休眠模式，nmsg = 0。
- [ ] `SDB`：工作模式，nmsg = 0。
- [x] `INT`：修改机器人舵机默认位置，nmsg = 4，分别为lup rup lturn rturn。示例：`$TSINT4:100;75;55;70;<`
- [ ] `FWD`：修改前进动作，nmsg >= 1，每个sct中包含4个val，分别为langle rangle upturnflag(0:up,1:turn) delay(ms)。示例：`$TSFWD1:15,160,0,200;<`
- [ ] `BWD`：修改后退动作，nmsg >= 1，同上。
- [ ] `FWS`：修改单个前进动作，nmsg = 1，每个sct中包含5个val，分别为index langle rangle upturnflag delay。
- [ ] `BWS`：修改单个后退动作，nmsg = 1，同上。
- [ ] `MVF`：前进，nmsg = 0。
- [ ] `MVB`：后退，nmsg = 0。
- [ ] `STP`：停止当前动作，nmsg = 0。
- [x] `DBG`：调试指令，nmsg = 1，每个sct中包含4个val，同`FWD`指令。示例：`$TSDBG1:15,160,0,200;<`
- [x] `SAV`：保存当前动作，nmsg = 0。

#### AC - Air Conditioner

- [x] `ONN`：打开空调，nmsg = 0时以默认温度和模式打开，nmsg = 1时需要在msg中给出开启温度。示例：`$ACONN1:27;<`
- [x] `OFF`：关闭空调，nmsg = 0。示例：`$ACOFF0:<`
- [x] `TPR`：调节温度，nmsg = 1，在msg中给出温度。示例：`$ACTPR1:27;<`
- [ ] `MOD`：调节模式，nmsg = 1，未实现
- [ ] `SPD`：调节风速，nmsg = 1，未实现

#### LT - Light
- [x] `ONN`：打开灯光，nmsg = 0。
- [x] `OFF`：关闭灯光，nmsg = 0。

#### PW - Power
- [x] `ONN`：打开电源，nmsg = 0。
- [x] `OFF`：关闭电源，nmsg = 0。

### Particle Photon Post Command Example
`$ curl https://api.particle.io/v1/devices/49002a001151373331333230/tsdbg -d access_token=04e6d2090d28e4b27a7daaa5f17c3a45fc404e56 -d "args=100,100,100,100;"`