#include "PivView.hpp"

PivView::PivView(PivEng::PivPoint::PivPointVec &vs)
    : m_pivPointVec(vs), piv_vectors(vs.size()) {
  get_piv_vectors_and_maximum_velocity();

  // auto unique_x_coords = get_unique_grid_values();
  // bpu::print_each(unique_x_coords, ", ");

  // std::vector<sf::VertexArray> vel_vector_graphics(num_vectors);

  sf::ContextSettings settings;
  sf::RenderWindow window(sf::VideoMode(612, 612), "BPPIV Vector Viewer",
                          sf::Style::Default, settings);
  window.setFramerateLimit(60);

  axis_bottom_left = sf::Vector2i(50, 562);
  axis_top_right = sf::Vector2i(562, 50);

  font.loadFromFile("../fonts/cour.ttf");
  make_graphics();
  make_status_bar_texts();
  make_header_texts();

  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      event_handler(window, event);
    }
    render_frame(window);
  }
}

PivView::~PivView() {}

void PivView::get_piv_vectors_and_maximum_velocity() {
  auto vector_magnitude = 0.0;
  auto counter = 0;
  for (auto &piv_point : m_pivPointVec) {
    auto piv_vector = piv_point.get_piv_vector();
    if (piv_vector) {
      vector_magnitude = piv_vector->get_magnitude();
      piv_vectors[counter++] = *piv_vector;
      if (vector_magnitude > max_velocity_magnitude)
        max_velocity_magnitude = vector_magnitude;
    }
  }
  piv_vectors.resize(counter);
  num_vectors = counter;
}

void PivView::make_graphics() {
  axis_box = make_axis_box(50, 50, 562, 562);
  crosshairs = make_crosshairs(50, 50, 562, 562);

  vel_vector_graphics.resize(num_vectors);
  auto vel_vector_graphic = vel_vector_graphics.begin();
  for (auto &piv_vector : piv_vectors) {
    *(vel_vector_graphic++) = make_vector_graphic(piv_vector);
  }
}

sf::Color PivView::vector_color(const double vel_magnitude,
                                const double max_vel_magnitude) const {
  auto magnitude_ratio = vel_magnitude / max_vel_magnitude;
  uint8_t red, green, blue;

  red = 255 - static_cast<uint8_t>(magnitude_ratio * 255);
  green = static_cast<uint8_t>(0);
  blue = static_cast<uint8_t>(magnitude_ratio * 255);

  return sf::Color(red, green, blue);
}

sf::VertexArray PivView::make_vector_graphic(PivEng::PivVector &piv_vector) {
  auto factor = 3;
  auto x = piv_vector.get_x(), y = piv_vector.get_y(), u = piv_vector.get_u(),
       v = piv_vector.get_v();

  sf::VertexArray vector_lines(sf::Lines, 2);

  vector_lines[0].position = sf::Vector2f(50 + x, 50 + y);
  vector_lines[1].position =
      sf::Vector2f(50 + x + factor * u, 50 + y + factor * v);

  auto mag = piv_vector.get_magnitude();
  sf::Color magnitude_color = vector_color(mag, max_velocity_magnitude);

  vector_lines[0].color = magnitude_color;
  vector_lines[1].color = magnitude_color;

  return vector_lines;
}

sf::VertexArray PivView::make_axis_box(const int i_min, const int j_min,
                                       const int i_max, const int j_max) const {
  auto axis_box = sf::VertexArray(sf::LinesStrip, 5);
  axis_box[0] = sf::Vector2f(i_min, j_max);
  axis_box[1] = sf::Vector2f(i_max, j_max);
  axis_box[2] = sf::Vector2f(i_max, j_min);
  axis_box[3] = sf::Vector2f(i_min, j_min);
  axis_box[4] = axis_box[0];
  return axis_box;
}

sf::VertexArray PivView::make_crosshairs(const int i_min, const int j_min,
                                         const int i_max,
                                         const int j_max) const {
  sf::VertexArray crosshairs(sf::Lines, 4);

  crosshairs[0].position.x = i_min;
  crosshairs[0].position.y = j_min;

  crosshairs[1].position.x = i_max;
  crosshairs[1].position.y = j_min;

  crosshairs[2].position.x = i_min;
  crosshairs[2].position.y = j_min;

  crosshairs[3].position.x = i_min;
  crosshairs[3].position.y = j_max;

  return crosshairs;
}

void PivView::update_crosshairs(const sf::Vector2f &mouse_converted) {
  crosshairs[0].position.y = mouse_converted.y;
  crosshairs[1].position.y = mouse_converted.y;
  crosshairs[2].position.x = mouse_converted.x;
  crosshairs[3].position.x = mouse_converted.x;
}

void PivView::render_frame(sf::RenderWindow &window) const {
  window.clear(sf::Color::Black);
  window.draw(header_text);
  window.draw(axis_box);
  window.draw(crosshairs);
  std::for_each(vel_vector_graphics.begin(), vel_vector_graphics.end(),
                [&](auto &dv) { window.draw(dv); });
  for (auto &label : status_bar_texts) {
    window.draw(label);
  }
  window.display();
}

void PivView::event_handler(sf::RenderWindow &window, const sf::Event &event) {
  switch (event.type) {
  case sf::Event::Closed:
    window.close();

  case sf::Event::Resized:
    render_frame(window);

  case sf::Event::MouseMoved:
    mouse = window.mapPixelToCoords(sf::Mouse::getPosition(window));
    if (mouse.x > 50 && mouse.x < 562 && mouse.y > 50 && mouse.y < 562) {
      update_status_bar(mouse);
      update_crosshairs(mouse);
      render_frame(window);
    }

  default:
    ;
  }
}

void PivView::update_status_bar(const sf::Vector2f &mouse) {
  status_bar_texts[1].setString(std::to_string(static_cast<int>(mouse.x - 50)));
  status_bar_texts[3].setString(std::to_string(static_cast<int>(mouse.y - 50)));
}

sf::Text PivView::make_status_bar_label(const int x_position,
                                        std::string &&words) {
  sf::Text label;
  label.setFont(font);
  label.setString(words);
  label.setCharacterSize(18);
  label.setColor(sf::Color::White);
  label.setPosition(x_position, 580);
  return label;
}

void PivView::make_status_bar_texts() {
  status_bar_texts.push_back(make_status_bar_label(10, "x:"));
  status_bar_texts.push_back(make_status_bar_label(35, ""));
  status_bar_texts.push_back(make_status_bar_label(90, "y:"));
  status_bar_texts.push_back(make_status_bar_label(115, ""));
}

void PivView::make_header_texts() {
  header_text.setFont(font);
  header_text.setString("BPPIV v0.1");
  header_text.setCharacterSize(24);
  header_text.setColor(sf::Color::White);
  header_text.setPosition(10, 5);
}

std::vector<double> PivView::get_unique_grid_values() {
  auto get_x = [](auto &pv) { return pv.get_x(); };
  return bpu::collect_unique(piv_vectors, get_x);
}
