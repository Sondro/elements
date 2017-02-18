/*=======================================================================================
   Copyright (c) 2016-2017 Joel de Guzman

   Distributed under the MIT License [ https://opensource.org/licenses/MIT ]
=======================================================================================*/
#include <photon/widget/gallery.hpp>
#include <photon/support/text_utils.hpp>
#include <photon/support/theme.hpp>

namespace photon
{
   void background_fill::draw(context const& ctx)
   {
      auto&  cnv = ctx.canvas;
      cnv.fill_style(_color);
      cnv.fill_rect(ctx.bounds);
   }

   widget_limits heading::limits(basic_context const& ctx) const
   {
      auto& thm = get_theme();
      auto  size = measure_text(
         ctx.canvas, _text.c_str(), thm.heading_font, thm.heading_font_size * _size
      );
      return { { size.x, size.y }, { size.x, size.y } };
   }

   void heading::draw(context const& ctx)
   {
      auto const&    theme_ = get_theme();
      auto&          canvas_ = ctx.canvas;
      auto           state = canvas_.new_state();

      canvas_.fill_style(theme_.heading_font_color);
      canvas_.font(theme_.heading_font, theme_.heading_font_size * _size, theme_.heading_style);
      canvas_.text_align(canvas_.middle | canvas_.center);

      float cx = ctx.bounds.left + (ctx.bounds.width() / 2);
      float cy = ctx.bounds.top + (ctx.bounds.height() / 2);

      canvas_.fill_text(point{ cx, cy }, _text.c_str());
   }

   void title_bar::draw(context const& ctx)
   {
      float const corner_radius = 4.0;
      auto&       canvas_ = ctx.canvas;
      auto const& bounds = ctx.bounds;

      auto gradient = canvas::linear_gradient{
         bounds.top_left(),
         bounds.bottom_left()
      };

      gradient.add_color_stop({ 0.0, { 255, 255, 255, 16 } });
      gradient.add_color_stop({ 1.0, { 0, 0, 0, 16 } });
      canvas_.fill_style(gradient);

      canvas_.begin_path();
      canvas_.round_rect(bounds, corner_radius);
      canvas_.fill();
      canvas_.fill();

      canvas_.begin_path();
      canvas_.move_to(point{ bounds.left+0.5f, bounds.bottom-0.5f });
      canvas_.line_to(point{ bounds.right-0.5f, bounds.bottom-0.5f });
      canvas_.stroke_style(color{ 0, 0, 0, 32 });
      canvas_.line_width(1);
      canvas_.stroke();
   }

   widget_limits label::limits(basic_context const& ctx) const
   {
      auto& thm = get_theme();
      auto  size = measure_text(
         ctx.canvas, _text.c_str(), thm.label_font, thm.label_font_size * _size
      );
      return { { size.x, size.y }, { size.x, size.y } };
   }

   void label::draw(context const& ctx)
   {
      auto const&    theme_ = get_theme();
      auto&          canvas_ = ctx.canvas;
      auto           state = canvas_.new_state();

      canvas_.fill_style(theme_.label_font_color);
      canvas_.font(theme_.label_font, theme_.label_font_size * _size, theme_.label_style);
      canvas_.text_align(canvas_.middle | canvas_.center);

      float cx = ctx.bounds.left + (ctx.bounds.width() / 2);
      float cy = ctx.bounds.top + (ctx.bounds.height() / 2);

      canvas_.fill_text(point{ cx, cy }, _text.c_str());
   }

   void panel::draw(context const& ctx)
   {
      auto const&    theme_ = get_theme();
      float const    corner_radius  = 4.0;
      rect const     shadow_offset  = { -10, -10, +20, +30 };
      auto&          canvas_ = ctx.canvas;
      auto const&    bounds = ctx.bounds;

      // Panel fill
      canvas_.begin_path();
      canvas_.round_rect(ctx.bounds, corner_radius);
      canvas_.fill_style(theme_.panel_color);
      canvas_.fill();
      
      // Simulated blurred shadow (cairo does not have blur yet)
      {
         auto save = canvas_.new_state();

         canvas_.begin_path();
         auto vs = ctx.view.size();
         canvas_.rect({ 0, 0, vs.x, vs.y });
         canvas_.round_rect(bounds.inset(0.5, 0.5), corner_radius);
         canvas_.fill_rule(canvas::fill_odd_even);
         canvas_.clip();

         canvas_.round_rect(bounds, corner_radius);
         canvas_.line_width(2);
         canvas_.stroke_style(color(0, 0, 0, 50));
         canvas_.stroke();

         rect shr = bounds;
         shr.left -= 2;
         shr.top -= 2;
         shr.right += 6;
         shr.bottom += 6;

         canvas_.begin_path();
         canvas_.round_rect(shr, corner_radius*2);
         canvas_.fill_style(color(0, 0, 0, 20));
         canvas_.fill();
         
         shr.left += 1;
         shr.top += 1;
         shr.right -= 2;
         shr.bottom -= 2;
         canvas_.begin_path();
         canvas_.round_rect(shr, corner_radius*1.5);
         canvas_.fill_style(color(0, 0, 0, 30));
         canvas_.fill();
         
         shr.left += 1;
         shr.top += 1;
         shr.right -= 2;
         shr.bottom -= 2;
         canvas_.begin_path();
         canvas_.round_rect(shr, corner_radius);
         canvas_.fill_style(color(0, 0, 0, 40));
         canvas_.fill();
      }


      //// Drop shadow
      //rect offs = shadow_offset;
      //canvas_.begin_path();
      //rect  shr =
      //   {  bounds.left+offs.left, bounds.top+offs.top,
      //      bounds.right+offs.right, bounds.bottom+offs.bottom
      //   };
      //
      //canvas_.rect(shr);
      //canvas_.round_rect(bounds, corner_radius);

//      canvas_.path_winding(canvas::hole);
//
//      paint shadow_paint
//         = canvas_.box_gradient(bounds.move(0, 2), corner_radius*2, 10
//          , color(0, 0, 0, 128), color(0, 0, 0, 0)
//         );
//
//      canvas_.fill_paint(shadow_paint);
//      canvas_.fill();
   }

   void frame::draw(context const& ctx)
   {
      auto const&    theme_ = get_theme();
      auto&          canvas_ = ctx.canvas;
      auto const&    bounds = ctx.bounds;

      canvas_.line_width(theme_.frame_stroke_width);
      canvas_.stroke_style(colors::black.opacity(0.4));
      canvas_.stroke_round_rect(bounds.move(-1, -1), theme_.frame_corner_radius);
      canvas_.stroke_style(theme_.frame_color);
      canvas_.stroke_round_rect(bounds, theme_.frame_corner_radius);
   }

   void vgrid_lines::draw(context const& ctx)
   {
      auto const&    theme_ = get_theme();
      auto&          canvas_ = ctx.canvas;
      auto const&    bounds = ctx.bounds;

      float pos = bounds.top;
      float incr = bounds.height() / _major_divisions;

      canvas_.stroke_style(theme_.major_grid_color);
      canvas_.line_width(theme_.major_grid_width);
      while (pos <= bounds.bottom+1)
      {
         canvas_.move_to({ bounds.left, pos });
         canvas_.line_to({ bounds.right, pos });
         canvas_.stroke();
         pos += incr;
      }

      pos = bounds.top;
      incr = bounds.height() / _minor_divisions;

      canvas_.stroke_style(theme_.minor_grid_color);
      canvas_.line_width(theme_.minor_grid_width);
      while (pos <= bounds.bottom+1)
      {
         canvas_.move_to({ bounds.left, pos });
         canvas_.line_to({ bounds.right, pos });
         canvas_.stroke();
         pos += incr;
      }
   }

   icon::icon(std::uint32_t code_, float size_)
    : _code(code_)
    , _size(size_)
   {}

   widget_limits icon::limits(basic_context const& ctx) const
   {
      auto& thm = get_theme();
      float font_size = (_size > 0)? _size : thm.icon_font_size;
      point s = measure_icon(ctx.canvas, _code, font_size);
      return { { s.x, s.y }, { s.x, s.y } };
   }

   void icon::draw(context const& ctx)
   {
      auto& thm = get_theme();
      float font_size = (_size > 0)? _size : thm.icon_font_size;
      draw_icon(ctx.canvas, ctx.bounds, _code, font_size);
   }

   float basic_button_body::corner_radius = 4.0;

   void draw_button_base(context const& ctx, rect bounds, color color_, float corner_radius)
   {
      canvas&  canvas_ = ctx.canvas;

      auto gradient = canvas::linear_gradient{
         bounds.top_left(),
         bounds.bottom_left()
      };

      gradient.add_color_stop({ 0.0, { 255, 255, 255, 32 } });
      gradient.add_color_stop({ 1.0, { 0, 0, 0, 32 } });
      canvas_.fill_style(gradient);

      canvas_.begin_path();
      canvas_.round_rect(bounds.inset(1, 1), corner_radius-1);
      canvas_.fill_style(color_);
      canvas_.fill();
      canvas_.round_rect(bounds.inset(1, 1), corner_radius-1);
      canvas_.fill_style(gradient);
      canvas_.fill();

      canvas_.begin_path();
      canvas_.round_rect(bounds.inset(0.5, 0.5), corner_radius-0.5);
      canvas_.stroke_style(color{ 0, 0, 0, 48 });
      canvas_.stroke();
   }

   void basic_button_body::draw(context const& ctx)
   {
      draw_button_base(ctx, ctx.bounds, body_color, corner_radius);
   }

   basic_button
   button(
      std::string const& text
    , color body_color
   )
   {
      return make_button<basic_button>(text, body_color);
   }

   basic_button
   button(
      std::uint32_t icon_code
    , std::string const& text
    , color body_color
   )
   {
      return make_button<basic_button>(icon_code, text, body_color);
   }

   basic_button
   button(
      std::string const& text
    , std::uint32_t icon_code
    , color body_color
   )
   {
      return make_button<basic_button>(text, icon_code, body_color);
   }

   basic_toggle_button
   toggle_button(
      std::string const& text
    , color body_color
   )
   {
      return make_button<basic_toggle_button>(text, body_color);
   }

   basic_toggle_button
   toggle_button(
      std::uint32_t icon_code
    , std::string const& text
    , color body_color
   )
   {
      return make_button<basic_toggle_button>(icon_code, text, body_color);
   }

   basic_toggle_button
   toggle_button(
      std::string const& text
    , std::uint32_t icon_code
    , color body_color
   )
   {
      return make_button<basic_toggle_button>(text, icon_code, body_color);
   }

   basic_latching_button
   latching_button(
      std::string const& text
    , color body_color
   )
   {
      return make_button<basic_latching_button>(text, body_color);
   }

   basic_latching_button
   latching_button(
      std::uint32_t icon_code
    , std::string const& text
    , color body_color
   )
   {
      return make_button<basic_latching_button>(icon_code, text, body_color);
   }

   basic_latching_button
   latching_button(
      std::string const& text
    , std::uint32_t icon_code
    , color body_color
   )
   {
      return make_button<basic_latching_button>(text, icon_code, body_color);
   }

   basic_popup_button
   popup_button(
      std::string const& text
    , color body_color
   )
   {
      return make_button<basic_popup_button>(text, icons::down_dir, body_color);
   }

   //void draw_check_box(
   //   context const& ctx, std::string const& text, bool state, bool hilite
   //)
   //{
   //   canvas&  canvas_ = ctx.canvas;
   //   theme const&   theme_ = get_theme();
   //   color    indicator_color = theme_.indicator_color;
   //   rect     box = ctx.bounds.move(15, 0);
   //
   //   box.width(box.height());
   //
   //   color c1 = state ? indicator_color.level(1.5) : color(0, 0, 0, 32);
   //   color c2 = state ? indicator_color : color(0, 0, 0, 92);
   //
   //   if (state && hilite)
   //   {
   //      c1 = c1.level(1.3);
   //      c2 = c2.level(1.3);
   //   }
   //   paint bg = canvas_.box_gradient(box.inset(1, 1).move(0, 1.5), 3, 4, c1, c2);
   //
   //   canvas_.begin_path();
   //   canvas_.round_rect(box.inset(1, 1), 3);
   //   canvas_.fill_paint(bg);
   //   canvas_.fill();
   //
   //   if (state || hilite)
   //   {
   //      float const glow = 2;
   //      color glow_color = hilite ? indicator_color : c2;
   //
   //      paint glow_paint
   //         = canvas_.box_gradient(box, 3, 3, glow_color, color(0, 0, 0, 0)
   //         );
   //
   //      canvas_.begin_path();
   //      canvas_.rect(box.inset(-glow, -glow));
   //      canvas_.round_rect(box.inset(1.5, 1.5), 3);
   //      canvas_.path_winding(canvas::hole);
   //      canvas_.fill_paint(glow_paint);
   //      canvas_.fill();
   //   }
   //
   //   if (state)
   //   {
   //      auto save = set(theme_.icon_color, c1.level(2.0));
   //      text_utils(theme_).draw_icon(box, icons::ok, 14);
   //   }
   //   else
   //   {
   //      color outline_color = hilite ? theme_.frame_color : color{ 0, 0, 0, 48 };
   //      canvas_.begin_path();
   //      canvas_.round_rect(box.inset(1, 1), 3);
   //      canvas_.stroke_color(outline_color);
   //      canvas_.stroke();
   //   }
   //
   //   rect  text_bounds = ctx.bounds.move(45, 0);
   //   text_utils(theme_)
   //      .draw_text(
   //         text_bounds, text.c_str(), theme_.label_font,
   //         theme_.label_font_size, theme_.label_font_color
   //      );
   //}

   void draw_icon_button(
      context const& ctx, uint32_t code, float size, bool state, bool hilite
   )
   {
      float corner_radius = 6;
      color indicator_color = get_theme().indicator_color.level(0.7);
      color body_color = indicator_color;

      if (state)
         body_color = body_color.level(2.0);
      else if (hilite)
         body_color = body_color.level(1.5);

      // Draw Button Body
      draw_button_base(ctx, ctx.bounds, body_color, corner_radius);

      canvas&        canvas_ = ctx.canvas;
      rect           bounds = ctx.bounds;

      // Draw Glow
//      if (state)
//      {
//         float const glow = 7;
//         color glow_color = indicator_color.level(2.0);
//
//         paint glow_paint
//               = canvas_.box_gradient(bounds, corner_radius, 8, glow_color, color(0, 0, 0, 0)
//            );
//
//         canvas_.begin_path();
//         canvas_.rect(bounds.inset(-glow, -glow));
//         canvas_.round_rect(bounds.inset(1.5, 1.5), corner_radius-1);
//         canvas_.path_winding(canvas::hole);
//         canvas_.fill_paint(glow_paint);
//         canvas_.fill();
//      }

      // Draw Icon
      color icon_color = state ? indicator_color.level(4.0) : indicator_color.level(0.2);
      draw_icon(canvas_, bounds.move(0.5, 0.5), code, size, icon_color);

   }

   namespace
   {
       void draw_menu_background(cairo_t& _context, rect bounds, float radius)
       {
          auto x = bounds.left;
          auto y = bounds.top;
          auto r = bounds.right;
          auto b = bounds.bottom;
          auto const a = M_PI/180.0;

          cairo_new_sub_path(&_context);
          cairo_move_to(&_context, r, y);
          cairo_arc(&_context, r-radius, b-radius, radius, 0*a, 90*a);
          cairo_arc(&_context, x+radius, b-radius, radius, 90*a, 180*a);
          cairo_line_to(&_context, x, y);
          cairo_close_path(&_context);
       }
   }

   void menu_background::draw(context const& ctx)
   {
      auto&       canvas_ = ctx.canvas;
      auto const& bounds = ctx.bounds;

      canvas_.begin_path();
      draw_menu_background(canvas_.cairo_context(), bounds, 5);
      canvas_.fill_style(get_theme().panel_color.opacity(0.95));
      canvas_.fill();
   }

   widget_limits menu_item_spacer_widget::limits(basic_context const& ctx) const
   {
      return { { 0, 0 }, { full_extent, get_theme().label_font_size } };
   }

   void menu_item_spacer_widget::draw(context const& ctx)
   {
      auto& canvas_ = ctx.canvas;
      float y = ctx.bounds.top + ctx.bounds.height() / 2;

      canvas_.begin_path();
      canvas_.move_to({ ctx.bounds.left, y });
      canvas_.line_to({ ctx.bounds.right, y });
      canvas_.stroke_style(get_theme().frame_color.opacity(0.15));
      canvas_.line_width(1);
      canvas_.stroke();
   }

   void input_panel::draw(context const& ctx)
   {
      auto&       canvas_ = ctx.canvas;
      auto&       bounds = ctx.bounds;
      auto const& theme = get_theme();

      canvas_.line_width(1.5);
      canvas_.stroke_style(colors::white.opacity(0.3));
      canvas_.round_rect(bounds, 4-0.5f);
      canvas_.stroke();

      canvas_.stroke_style(colors::black.opacity(0.5));
      canvas_.line_width(1);
      canvas_.round_rect(bounds.move(-1, -1), 4-0.5f);
      canvas_.stroke();

      canvas_.round_rect(bounds.move(-0.5, -0.5), 4-0.5f);
      canvas_.fill_style(theme.edit_box_fill_color.opacity(0.9));
      canvas_.fill();
   }
}
