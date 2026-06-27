using System;
using System.ComponentModel;
using System.Drawing;
using System.Windows.Forms;

namespace control_software.Controls
{
    /// <summary>
    /// 深色主题 ComboBox — 运行时暗色 OwnerDraw，设计时系统默认外观
    /// </summary>
    public class ModernComboBox : ComboBox
    {
        private const int WM_CTLCOLORLISTBOX = 0x0134;

        public ModernComboBox()
        {
            if (DesignMode || LicenseManager.UsageMode == LicenseUsageMode.Designtime)
            {
                // 设计时：系统默认外观，在 VS 预览窗正常显示
                DropDownStyle = ComboBoxStyle.DropDownList;
                BackColor = SystemColors.Window;
                ForeColor = SystemColors.WindowText;
                Font = new Font("Segoe UI", 10F, FontStyle.Regular);
            }
            else
            {
                DrawMode = DrawMode.OwnerDrawFixed;
                DropDownStyle = ComboBoxStyle.DropDownList;
                FlatStyle = FlatStyle.Flat;
                BackColor = ThemeColors.SurfaceLight;
                ForeColor = ThemeColors.TextPrimary;
                Font = new Font("Segoe UI", 10F, FontStyle.Regular);
                ItemHeight = 24;
                SetStyle(ControlStyles.UserPaint | ControlStyles.AllPaintingInWmPaint |
                         ControlStyles.ResizeRedraw | ControlStyles.OptimizedDoubleBuffer, true);
            }
        }

        protected override void OnDrawItem(DrawItemEventArgs e)
        {
            if (DesignMode || LicenseManager.UsageMode == LicenseUsageMode.Designtime)
            {
                base.OnDrawItem(e);
                return;
            }
            if (e.Index < 0) return;

            e.DrawBackground();
            bool isSelected = (e.State & DrawItemState.Selected) != 0;

            using (var bgBrush = new SolidBrush(isSelected ? ThemeColors.Accent : ThemeColors.SurfaceLight))
            {
                e.Graphics.FillRectangle(bgBrush, e.Bounds);
            }

            string text = Items[e.Index].ToString();
            var textRect = new Rectangle(e.Bounds.X + 4, e.Bounds.Y + 2,
                e.Bounds.Width - 8, e.Bounds.Height - 4);
            TextRenderer.DrawText(e.Graphics, text, Font, textRect,
                isSelected ? Color.White : ThemeColors.TextPrimary, Color.Transparent,
                TextFormatFlags.Left | TextFormatFlags.VerticalCenter);

            base.OnDrawItem(e);
        }

        protected override void OnPaint(PaintEventArgs e)
        {
            if (DesignMode || LicenseManager.UsageMode == LicenseUsageMode.Designtime)
            {
                base.OnPaint(e);
                return;
            }

            e.Graphics.SmoothingMode = System.Drawing.Drawing2D.SmoothingMode.AntiAlias;

            using (var bgBrush = new SolidBrush(BackColor))
                e.Graphics.FillRectangle(bgBrush, ClientRectangle);

            if (SelectedItem != null)
            {
                string text = SelectedItem.ToString();
                var textRect = new Rectangle(ClientRectangle.X + 4, ClientRectangle.Y + 2,
                    ClientRectangle.Width - 22, ClientRectangle.Height - 4);
                TextRenderer.DrawText(e.Graphics, text, Font, textRect,
                    ForeColor, Color.Transparent,
                    TextFormatFlags.Left | TextFormatFlags.VerticalCenter);
            }

            // 下拉箭头
            int ax = ClientRectangle.Width - 16, ay = ClientRectangle.Height / 2 - 2;
            using (var arrowPen = new Pen(ThemeColors.TextSecondary, 2f))
            {
                e.Graphics.DrawLine(arrowPen, ax, ay, ax + 6, ay);
                e.Graphics.DrawLine(arrowPen, ax + 1, ay + 1, ax + 5, ay + 1);
                e.Graphics.DrawLine(arrowPen, ax + 2, ay + 2, ax + 4, ay + 2);
                e.Graphics.DrawLine(arrowPen, ax + 3, ay + 3, ax + 3, ay + 3);
            }

            using (var borderPen = new Pen(ThemeColors.Border, 1f))
                e.Graphics.DrawRectangle(borderPen, 0, 0, ClientRectangle.Width - 1, ClientRectangle.Height - 1);
        }

        protected override void WndProc(ref Message m)
        {
            if (m.Msg == WM_CTLCOLORLISTBOX && !DesignMode) { /* 阻止系统刷白下拉列表 */ }
            base.WndProc(ref m);
        }

        protected override void OnDropDownClosed(EventArgs e)
        {
            if (!DesignMode) Invalidate();
            base.OnDropDownClosed(e);
        }
    }
}
