using System.Drawing;

namespace control_software
{
    /// <summary>
    /// "Aurora Cockpit" 车载仪表配色方案
    /// </summary>
    public static class ThemeColors
    {
        // ═══════ 背景层级 ═══════
        public static readonly Color CockpitBg    = Color.FromArgb(0x0B, 0x0F, 0x17);  // #0B0F17 最深仪表黑
        public static readonly Color PanelBg      = Color.FromArgb(0x14, 0x19, 0x26);  // #141926 仪表面板底
        public static readonly Color Surface      = Color.FromArgb(0x1A, 0x20, 0x30);  // #1A2030 卡片表面
        public static readonly Color SurfaceLight = Color.FromArgb(0x21, 0x28, 0x3A);  // #21283A 输入框/交替行

        // ═══════ 边框 ═══════
        public static readonly Color Border       = Color.FromArgb(0x26, 0x2E, 0x42);  // #262E42

        // ═══════ 功能色 ═══════
        public static readonly Color Accent       = Color.FromArgb(0x4B, 0x9F, 0xFF);  // #4B9FFF 仪表蓝（主色调）
        public static readonly Color AccentHover  = Color.FromArgb(0x7A, 0xB8, 0xFF);  // 悬停
        public static readonly Color Success      = Color.FromArgb(0x34, 0xD3, 0x99);  // #34D399 前进/运行/解除
        public static readonly Color SuccessHover = Color.FromArgb(0x5E, 0xE0, 0xB2);  // 悬停
        public static readonly Color Danger       = Color.FromArgb(0xF8, 0x71, 0x71);  // #F87171 停止/制动
        public static readonly Color DangerHover  = Color.FromArgb(0xFE, 0xA0, 0xA0);  // 悬停
        public static readonly Color Warning      = Color.FromArgb(0xFB, 0xBF, 0x24);  // #FBBF24 后退/警告
        public static readonly Color WarningHover = Color.FromArgb(0xFC, 0xD3, 0x4D);  // 悬停
        public static readonly Color Info         = Color.FromArgb(0x38, 0xBD, 0xF8);  // #38BDF8 左转/右转/信息
        public static readonly Color InfoHover    = Color.FromArgb(0x7D, 0xD3, 0xFC);  // 悬停

        // ═══════ 文字 ═══════
        public static readonly Color TextPrimary   = Color.FromArgb(0xE6, 0xED, 0xF3);  // #E6EDF3
        public static readonly Color TextSecondary = Color.FromArgb(0x8B, 0x9B, 0xB4);  // #8B9BB4
        public static readonly Color TextDim       = Color.FromArgb(0x5A, 0x68, 0x80);  // #5A6880

        // ═══════ 按钮状态 ═══════
        public static readonly Color ButtonNormal  = Color.FromArgb(0x1E, 0x26, 0x38);  // 默认
        public static readonly Color ButtonHover   = Color.FromArgb(0x2A, 0x34, 0x4A);  // 悬停
        public static readonly Color ButtonPressed = Color.FromArgb(0x12, 0x18, 0x24);  // 按下

        // ═══════ 状态指示灯 ═══════
        public static readonly Color StatusOnline  = Color.FromArgb(0x34, 0xD3, 0x99);  // 在线/运行
        public static readonly Color StatusOffline = Color.FromArgb(0x5A, 0x68, 0x80);  // 离线/停止
        public static readonly Color StatusAlert   = Color.FromArgb(0xF8, 0x71, 0x71);  // 报警

        // 兼容旧属性名
        public static Color Background { get { return CockpitBg; } }
    }
}
