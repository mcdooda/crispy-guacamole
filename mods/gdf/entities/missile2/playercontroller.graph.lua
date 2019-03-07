return {
  links = {
    {
      1,
      1,
      2,
      1,
    },
    {
      1,
      2,
      3,
      1,
    },
    {
      1,
      3,
      3,
      2,
    },
  },
  nodeType = 'script',
  nodes = {
    {
      name = 'components/playercontrollercomponent',
    },
    {
      loadArguments = {
        {
          5,
          'Component',
        },
      },
      name = 'script/scriptoutput',
    },
    {
      loadArguments = {
        1,
        {
          links = {
            {
              1,
              1,
              10,
              2,
            },
            {
              2,
              2,
              6,
              2,
            },
            {
              3,
              1,
              2,
              1,
            },
            {
              5,
              1,
              4,
              1,
            },
            {
              6,
              1,
              4,
              2,
            },
            {
              7,
              1,
              6,
              1,
            },
            {
              8,
              1,
              2,
              2,
            },
            {
              9,
              1,
              8,
              1,
            },
            {
              10,
              2,
              8,
              2,
            },
            {
              11,
              1,
              10,
              1,
            },
          },
          nodeType = 'statemachine',
          nodes = {
            {
              name = 'statemachine/enter',
            },
            {
              loadArguments = {
                2,
                1,
                {
                  links = {
                    {
                      1,
                      1,
                      5,
                      1,
                    },
                    {
                      2,
                      1,
                      8,
                      1,
                    },
                    {
                      3,
                      1,
                      16,
                      2,
                    },
                    {
                      4,
                      1,
                      6,
                      1,
                    },
                    {
                      5,
                      1,
                      2,
                      1,
                    },
                    {
                      5,
                      2,
                      9,
                      1,
                    },
                    {
                      6,
                      1,
                      7,
                      1,
                    },
                    {
                      7,
                      3,
                      5,
                      2,
                    },
                    {
                      8,
                      1,
                      15,
                      1,
                    },
                    {
                      9,
                      1,
                      13,
                      2,
                    },
                    {
                      10,
                      1,
                      12,
                      1,
                    },
                    {
                      11,
                      1,
                      10,
                      1,
                    },
                    {
                      12,
                      3,
                      13,
                      1,
                    },
                    {
                      13,
                      1,
                      8,
                      2,
                    },
                    {
                      14,
                      1,
                      9,
                      2,
                    },
                    {
                      16,
                      1,
                      19,
                      2,
                    },
                    {
                      17,
                      1,
                      15,
                      2,
                    },
                    {
                      17,
                      1,
                      18,
                      1,
                    },
                    {
                      18,
                      1,
                      19,
                      1,
                    },
                    {
                      19,
                      1,
                      15,
                      3,
                    },
                    {
                      20,
                      1,
                      16,
                      1,
                    },
                  },
                  nodeType = 'script',
                  nodes = {
                    {
                      name = 'script/init',
                    },
                    {
                      name = 'flow/eachframe',
                    },
                    {
                      name = 'game/gamedt',
                    },
                    {
                      name = 'script/getcontext',
                    },
                    {
                      name = 'script/holdvalue',
                    },
                    {
                      name = 'entity/getposition',
                    },
                    {
                      name = 'math/vector3/vector3split',
                    },
                    {
                      name = 'script/condition',
                    },
                    {
                      name = 'math/add',
                    },
                    {
                      name = 'entity/getposition',
                    },
                    {
                      name = 'script/getcontext',
                    },
                    {
                      name = 'math/vector3/vector3split',
                    },
                    {
                      name = 'math/greaterthan',
                    },
                    {
                      loadArguments = {
                        0.5,
                      },
                      name = 'number',
                    },
                    {
                      name = 'entity/setelevation',
                    },
                    {
                      name = 'math/multiply',
                    },
                    {
                      name = 'script/getcontext',
                    },
                    {
                      name = 'entity/getelevation',
                    },
                    {
                      name = 'math/add',
                    },
                    {
                      loadArguments = {
                        -7,
                      },
                      name = 'number',
                    },
                  },
                },
              },
              name = 'statemachine/state',
            },
            {
              loadArguments = {
                'move up',
              },
              name = 'string',
            },
            {
              loadArguments = {
                2,
                2,
                {
                  links = {
                    {
                      1,
                      1,
                      2,
                      2,
                    },
                    {
                      1,
                      1,
                      6,
                      1,
                    },
                    {
                      3,
                      1,
                      7,
                      2,
                    },
                    {
                      4,
                      1,
                      5,
                      1,
                    },
                    {
                      5,
                      1,
                      2,
                      1,
                    },
                    {
                      6,
                      1,
                      7,
                      1,
                    },
                    {
                      7,
                      1,
                      2,
                      3,
                    },
                    {
                      8,
                      1,
                      3,
                      2,
                    },
                    {
                      9,
                      1,
                      3,
                      1,
                    },
                  },
                  nodeType = 'script',
                  nodes = {
                    {
                      name = 'script/getcontext',
                    },
                    {
                      name = 'entity/setelevation',
                    },
                    {
                      name = 'math/multiply',
                    },
                    {
                      name = 'script/init',
                    },
                    {
                      name = 'flow/eachframe',
                    },
                    {
                      name = 'entity/getelevation',
                    },
                    {
                      name = 'math/add',
                    },
                    {
                      name = 'game/gamedt',
                    },
                    {
                      loadArguments = {
                        -2.5,
                      },
                      name = 'number',
                    },
                  },
                },
              },
              name = 'statemachine/state',
            },
            {
              loadArguments = {
                'move down',
              },
              name = 'string',
            },
            {
              loadArguments = {
                2,
                3,
                {
                  links = {
                    {
                      1,
                      1,
                      2,
                      2,
                    },
                    {
                      2,
                      1,
                      5,
                      1,
                    },
                    {
                      3,
                      1,
                      2,
                      1,
                    },
                    {
                      4,
                      1,
                      3,
                      1,
                    },
                  },
                  nodeType = 'script',
                  nodes = {
                    {
                      loadArguments = {
                        '0',
                      },
                      name = 'math/formula',
                    },
                    {
                      name = 'math/lessthan',
                    },
                    {
                      name = 'entity/getelevation',
                    },
                    {
                      name = 'script/getcontext',
                    },
                    {
                      loadArguments = {
                        {
                          1,
                          'Result',
                        },
                      },
                      name = 'script/scriptoutput',
                    },
                  },
                },
              },
              name = 'statemachine/rule',
            },
            {
              loadArguments = {
                'high enough',
              },
              name = 'string',
            },
            {
              loadArguments = {
                2,
                6,
                {
                  links = {
                    {
                      1,
                      1,
                      4,
                      1,
                    },
                    {
                      2,
                      1,
                      1,
                      2,
                    },
                    {
                      3,
                      1,
                      1,
                      1,
                    },
                  },
                  nodeType = 'script',
                  nodes = {
                    {
                      name = 'entity/iscomponentenabled',
                    },
                    {
                      loadArguments = {
                        'projectile',
                      },
                      name = 'string',
                    },
                    {
                      name = 'script/getcontext',
                    },
                    {
                      loadArguments = {
                        {
                          1,
                          'Is Enabled',
                        },
                      },
                      name = 'script/scriptoutput',
                    },
                  },
                },
              },
              name = 'statemachine/rule',
            },
            {
              loadArguments = {
                'projectile enabled',
              },
              name = 'string',
            },
            {
              loadArguments = {
                2,
                5,
                {
                  links = {
                    {
                      1,
                      1,
                      2,
                      1,
                    },
                    {
                      3,
                      1,
                      2,
                      3,
                    },
                    {
                      4,
                      1,
                      2,
                      2,
                    },
                  },
                  nodeType = 'script',
                  nodes = {
                    {
                      name = 'script/init',
                    },
                    {
                      name = 'entity/setelevation',
                    },
                    {
                      loadArguments = {
                        'pi / 2',
                      },
                      name = 'math/formula',
                    },
                    {
                      name = 'script/getcontext',
                    },
                  },
                },
              },
              name = 'statemachine/state',
            },
            {
              loadArguments = {
                'initialize direction',
              },
              name = 'string',
            },
          },
        },
      },
      name = 'statemachine',
    },
  },
}