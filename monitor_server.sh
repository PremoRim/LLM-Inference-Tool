#!/bin/bash
# 服务器资源监控脚本（2核2G专用）
# 功能：每10秒输出CPU/内存/磁盘占用，保存到日志文件

# 定义日志文件路径
LOG_FILE="./server_monitor.log"

# 清空旧日志（可选）
> $LOG_FILE

# 打印表头（美化输出）
echo -e "===== 服务器资源监控日志 | 开始时间：$(date +'%Y-%m-%d %H:%M:%S') =====" >> $LOG_FILE
echo -e "时间\t\tCPU占用(%)\t内存已用/总(GB)\t磁盘已用/总(GB)" >> $LOG_FILE
echo -e "---------------------------------------------" >> $LOG_FILE

# 无限循环监控（按Ctrl+C停止）
while true; do
    # 1. 获取当前时间
    CURRENT_TIME=$(date +'%Y-%m-%d %H:%M:%S')
    
    # 2. 获取CPU占用率（提取top命令的CPU行，计算空闲外的占比）
    CPU_USAGE=$(top -b -n 1 | grep '%Cpu(s)' | awk '{print 100-$8}' | cut -d. -f1)
    
    # 3. 获取内存占用（free -h提取Mem行，格式化输出）
    MEM_USAGE=$(free -h | grep Mem | awk '{print $3"/"$2}')
    
    # 4. 获取磁盘占用（df -h提取/根目录行，格式化输出）
    DISK_USAGE=$(df -h | grep '/$' | awk '{print $3"/"$1}')
    
    # 5. 输出到日志+屏幕
    echo -e "$CURRENT_TIME\t$CPU_USAGE%\t\t$MEM_USAGE\t\t$DISK_USAGE" | tee -a $LOG_FILE
    
    # 6. 每10秒执行一次
    sleep 10
done
