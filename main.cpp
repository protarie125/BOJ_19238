#include <algorithm>
#include <bit>
#include <chrono>
#include <cmath>
#include <cstdint>
#include <deque>
#include <iomanip>
#include <iostream>
#include <limits>
#include <map>
#include <numeric>
#include <queue>
#include <ranges>
#include <set>
#include <sstream>
#include <stack>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

using namespace std;

using ll = int64_t;
using vl = vector<ll>;
using vvl = vector<vl>;
using vb = vector<bool>;
using vvb = vector<vb>;
using Pos = pair<ll, ll>;
using vP = vector<Pos>;

ll N, M, F;
vvl Chi;
ll TR, TC;
vP Beg, End;

vl dr{-1, 0, 0, 1};
vl dc{0, -1, 1, 0};
vb bp;
tuple<ll, ll> FindPerson() {
  queue<tuple<ll, ll, ll>> Q{};
  Q.push({TR, TC, 0});

  auto visit = vvb(N, vb(N, false));
  visit[TR][TC] = true;

  auto mndst = 987654321;
  auto found = vector<tuple<ll, ll, ll, ll>>{};
  while (!Q.empty()) {
    auto [fr, fc, fd] = Q.front();
    Q.pop();

    if (fd > mndst) continue;

    for (auto i = 0; i < M; ++i) {
      if (bp[i]) continue;

      if (Beg[i].first == fr && Beg[i].second == fc) {
        mndst = fd;
        found.push_back({fr, fc, i, fd});
      }
    }

    for (auto d = 0; d < 4; ++d) {
      const auto& nr = fr + dr[d];
      const auto& nc = fc + dc[d];

      if (nr < 0 || N <= nr || nc < 0 || N <= nc) continue;
      if (visit[nr][nc]) continue;
      if (Chi[nr][nc] == 1) continue;

      Q.push({nr, nc, fd + 1});
      visit[nr][nc] = true;
    }
  }

  if (found.size() == 0) {
    return {-1, -1};
  }

  sort(found.begin(), found.end());
  const auto& [rr, rc, ri, rd] = found[0];
  return {ri, rd};
}

ll CalcDestination(ll idx) {
  queue<tuple<ll, ll, ll>> Q{};
  Q.push({TR, TC, 0});

  auto visit = vvb(N, vb(N, false));
  visit[TR][TC] = true;

  while (!Q.empty()) {
    auto [fr, fc, fd] = Q.front();
    Q.pop();

    if (End[idx].first == fr && End[idx].second == fc) return fd;

    for (auto d = 0; d < 4; ++d) {
      const auto& nr = fr + dr[d];
      const auto& nc = fc + dc[d];

      if (nr < 0 || N <= nr || nc < 0 || N <= nc) continue;
      if (visit[nr][nc]) continue;
      if (Chi[nr][nc] == 1) continue;

      Q.push({nr, nc, fd + 1});
      visit[nr][nc] = true;
    }
  }

  return -1;
}

ll solve() {
  ll catched = 0;
  bp = vb(M, false);
  while (catched < M) {
    const auto& [idx, d] = FindPerson();
    if (idx == -1) {
      return -1;
    }

    if (F < d) return -1;

    F -= d;
    TR = Beg[idx].first;
    TC = Beg[idx].second;
    bp[idx] = true;

    const auto& dst = CalcDestination(idx);
    if (dst == -1) return -1;

    if (F < dst) return -1;

    F += dst;
    TR = End[idx].first;
    TC = End[idx].second;

    ++catched;
  }

  return F;
}

int main() {
  ios::sync_with_stdio(false);
  std::cin.tie(nullptr);

  cin >> N >> M >> F;
  Chi = vvl(N, vl(N));
  for (auto&& row : Chi) {
    for (auto&& c : row) cin >> c;
  }
  cin >> TR >> TC;
  --TR;
  --TC;
  Beg = vP(M);
  End = vP(M);
  for (auto i = 0; i < M; ++i) {
    cin >> Beg[i].first >> Beg[i].second >> End[i].first >> End[i].second;
    Beg[i].first -= 1;
    Beg[i].second -= 1;
    End[i].first -= 1;
    End[i].second -= 1;
  }

  const auto& ans = solve();
  cout << ans;

  return 0;
}