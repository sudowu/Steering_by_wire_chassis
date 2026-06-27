using System;
using System.ComponentModel;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.Windows.Forms;

namespace control_software.Controls
{
    /// <summary>
    /// 现代圆角分组框 — 运行时暗色主题，设计时系统默认外观
    /// </summary>
    public class ModernGroupBox : GroupBox
    {
        private int _borderRadius = 8;

        public ModernGroupBox()
        {
            if (DesignMode || LicenseManager.UsageMode == LicenseUsageMode.Designtime)
            {
                // 设计时：系统默认外观，在 VS 预览窗正常显示
                BackColor = SystemColors.Control;
                ForeColor = SystemColors.ControlText;
                Font = new Font("Segoe UI", 10F, FontStyle.Regular);
            }
            else
            {
                SetStyle(ControlStyles.UserPaint | ControlStyles.AllPaintingInWmPaint |
                         ControlStyles.ResizeRedraw | ControlStyles.OptimizedDoubleBuffer, true);
                BackColor = ThemeColors.PanelBg;
                ForeColor = ThemeColors.Accent;
                Font = new Font("Segoe UI", 10F, FontStyle.Regular);
            }
        }

        protected override void OnPaint(PaintEventArgs e)
        {
            if (DesignMode || LicenseManager.UsageMode == LicenseUsageMode.Designtime)
            {
                base.OnPaint(e);
                return;
            }

            e.Graphics.SmoothingMode = SmoothingMode.AntiAlias;
            e.Graphics.PixelOffsetMode = PixelOffsetMode.HighQuality;

            using (var bgBrush = new SolidBrush(BackColor))
                e.Graphics.FillRectangle(bgBrush, ClientRectangle);

            SizeF titleSize = e.Graphics.MeasureString(Text, Font);
            int titleHeight = (int)titleSize.Height + 4;

            Rectangle rect = new Rectangle(0, titleHeight / 2,
                ClientRectangle.Width - 1, ClientRectangle.Height - 1 - titleHeight / 2);

            using (var path = GetRoundedRect(rect, _borderRadius))
            using (var pen = new Pen(ThemeColors.Border, 1f))
                e.Graphics.DrawPath(pen, path);

            var titleRect = new RectangleF(10, 0, titleSize.Width + 8, titleHeight);
            using (var titleBgBrush = new SolidBrush(BackColor))
                e.Graphics.FillRectangle(titleBgBrush, titleRect);

            using (var titleBrush = new SolidBrush(ForeColor))
                e.Graphics.DrawString(Text, Font, titleBrush, 14, (titleHeight - titleSize.Height) / 2);
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
    }
}
