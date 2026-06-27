using System;
using System.ComponentModel;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.Windows.Forms;

namespace control_software.Controls
{
    /// <summary>
    /// 现代圆角扁平化按钮 — 运行时暗色主题，设计时系统默认外观
    /// </summary>
    public class ModernButton : Button
    {
        private int _borderRadius = 6;
        private Color _normalColor = ThemeColors.ButtonNormal;
        private Color _hoverColor = ThemeColors.ButtonHover;
        private Color _pressedColor = ThemeColors.ButtonPressed;
        private Color _accentColor = ThemeColors.Accent;
        private Color _textColor = ThemeColors.TextPrimary;
        private Color _borderColor = ThemeColors.Border;
        private bool _isHovered, _isPressed;

        public int BorderRadius
        {
            get => _borderRadius;
            set { _borderRadius = value; Invalidate(); }
        }

        public Color AccentColor
        {
            get => _accentColor;
            set { _accentColor = value; Invalidate(); }
        }

        public bool UseAccent { get; set; } = false;

        public ModernButton()
        {
            if (DesignMode || LicenseManager.UsageMode == LicenseUsageMode.Designtime)
            {
                // 设计时：保持系统默认外观，在 VS 预览窗可见
                FlatStyle = FlatStyle.Standard;
                BackColor = SystemColors.Control;
                ForeColor = SystemColors.ControlText;
                Font = new Font("Segoe UI", 10F, FontStyle.Regular);
            }
            else
            {
                FlatStyle = FlatStyle.Flat;
                FlatAppearance.BorderSize = 0;
                BackColor = _normalColor;
                ForeColor = _textColor;
                Font = new Font("Segoe UI", 10F, FontStyle.Regular);
                Cursor = Cursors.Hand;
                SetStyle(ControlStyles.UserPaint | ControlStyles.AllPaintingInWmPaint |
                         ControlStyles.ResizeRedraw | ControlStyles.OptimizedDoubleBuffer, true);
            }
        }

        protected override void OnMouseEnter(EventArgs e) { if (!DesignMode) { _isHovered = true; Invalidate(); } base.OnMouseEnter(e); }
        protected override void OnMouseLeave(EventArgs e) { _isHovered = false; _isPressed = false; Invalidate(); base.OnMouseLeave(e); }
        protected override void OnMouseDown(MouseEventArgs e) { if (!DesignMode) { _isPressed = true; Invalidate(); } base.OnMouseDown(e); }
        protected override void OnMouseUp(MouseEventArgs e) { _isPressed = false; Invalidate(); base.OnMouseUp(e); }

        protected override void OnPaint(PaintEventArgs e)
        {
            if (DesignMode || LicenseManager.UsageMode == LicenseUsageMode.Designtime)
            {
                base.OnPaint(e);
                return;
            }

            base.OnPaint(e);
            e.Graphics.SmoothingMode = SmoothingMode.AntiAlias;
            e.Graphics.PixelOffsetMode = PixelOffsetMode.HighQuality;

            Color bg;
            if (!Enabled) bg = Color.FromArgb(80, 80, 80);
            else if (_isPressed) bg = _pressedColor;
            else if (_isHovered) bg = UseAccent ? LightenColor(_accentColor, 0.15f) : _hoverColor;
            else if (UseAccent) bg = _accentColor;
            else bg = _normalColor;

            using (var path = GetRoundedRect(ClientRectangle, _borderRadius))
            using (var brush = new SolidBrush(bg))
                e.Graphics.FillPath(brush, path);

            Color borderDraw = UseAccent && !_isHovered ? _accentColor : _borderColor;
            using (var path = GetRoundedRect(ClientRectangle, _borderRadius))
            using (var pen = new Pen(borderDraw, 1f))
                e.Graphics.DrawPath(pen, path);

            Color textColor = (!Enabled) ? ThemeColors.TextSecondary :
                              (UseAccent) ? Color.White : _textColor;
            TextRenderer.DrawText(e.Graphics, Text, Font, ClientRectangle,
                textColor, Color.Transparent,
                TextFormatFlags.HorizontalCenter | TextFormatFlags.VerticalCenter);
        }

        private GraphicsPath GetRoundedRect(Rectangle rect, int radius)
        {
            int r = Math.Min(radius, Math.Min(rect.Width, rect.Height) / 2);
            var path = new GraphicsPath();
            if (r <= 0) { path.AddRectangle(rect); return path; }
            int d = r * 2;
            path.AddArc(rect.X, rect.Y, d, d, 180, 90);
            path.AddArc(rect.X + rect.Width - d, rect.Y, d, d, 270, 90);
            path.AddArc(rect.X + rect.Width - d, rect.Y + rect.Height - d, d, d, 0, 90);
            path.AddArc(rect.X, rect.Y + rect.Height - d, d, d, 90, 90);
            path.CloseFigure();
            return path;
        }

        private static Color LightenColor(Color color, float factor)
        {
            int r = Math.Min(255, (int)(color.R + (255 - color.R) * factor));
            int g = Math.Min(255, (int)(color.G + (255 - color.G) * factor));
            int b = Math.Min(255, (int)(color.B + (255 - color.B) * factor));
            return Color.FromArgb(r, g, b);
        }
    }
}
